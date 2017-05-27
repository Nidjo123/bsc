#include <cmath>
#include <climits>
#include "SemiGlobalMatching.hpp"

struct Path {
  int dx;
  int dy;

  Path(int dx_, int dy_) : dx(dx_), dy(dy_) {}
};

const Path paths[MAX_PATHS] = {
  // horizontal and vertical
  Path(-1, 0), Path(+1, 0), Path(0, -1), Path(0, +1),
  // diagonal
  Path(-1, -1), Path(+1, -1), Path(+1, +1), Path(-1, +1),
  // between diagonals (one step hor. or vert. then one step diagonal
  Path(-2, -1), Path(-1, -2), Path(+1, -2), Path(+2, -1),
  Path(+2, +1), Path(+1, +2), Path(-1, +2), Path(-2, +1),
};

SemiGlobalMatching::SemiGlobalMatching(Correspondence *correspondence, int P1, int P2)
  : Matching(correspondence), P1_(P1), P2_(P2) {
  S_ = new int**[height_];
  
  for (int i = 0; i < height_; i++) {
    S_[i] = new int*[width_];

    for (int j = 0; j < width_; j++) {
      S_[i][j] = new int[max_disparity_ + 1];
      for (int k = 0; k < max_disparity_ + 1; k++) {
	S_[i][j][k] = 0;
      }
    }
  }

  L_[0] = new int[max_disparity_ + 1];
  L_[1] = new int[max_disparity_ + 1];
}

SemiGlobalMatching::~SemiGlobalMatching() {
  // delete S and disparity map

  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      delete [] S_[y][x];
    }
    delete [] S_[y];
  }

  delete [] S_;

  delete [] L_[0];
  delete [] L_[1];
}

bool SemiGlobalMatching::isBorderPixel(int x, int y) const {
  return x == 0 || y == 0 || x == width_ - 1 || y == height_ - 1;
}

void SemiGlobalMatching::traversePath(int sx, int sy, const Path &path, int ***C) {
  if (sx < 0 || sy < 0 || sx >= width_ || sy >= height_ || !isBorderPixel(sx, sy)) return;

  for (int d = 0; d < max_disparity_ + 1; d++) {
    L_[0][d] = C[sy][sx][d];
  }

  int x = sx + path.dx;
  int y = sy + path.dy;
  int min_prev_Lr = C[sy][sx][0];
  
  for (; x > 0 && y > 0 && x < width_ - 1 && y < height_ - 1; x += path.dx, y += path.dy) {
    int min_Lr = INT_MAX;
    
    for (int d = 0; d < max_disparity_ + 1; d++) {
      int Lr = INT_MAX;

      if (Lr > L_[0][d]) {
	Lr = L_[0][d];
      }
      if (d > 0 && Lr > L_[0][d-1] + P1_) {
	Lr = L_[0][d-1] + P1_;
      }
      if (d < max_disparity_ && Lr > L_[0][d+1] + P1_) {
	Lr = L_[0][d+1] + P1_;
      }
      if (Lr > min_prev_Lr + P2_) {
	Lr = min_prev_Lr + P2_;
      }

      S_[y][x][d] += (L_[1][d] = C[y][x][d] + Lr - min_prev_Lr);
      min_Lr = std::min(min_Lr, L_[1][d]);
    }

    min_prev_Lr = min_Lr;

    for (int i = 0; i < max_disparity_ + 1; i++) {
      L_[0][i] = L_[1][i];
    }
  }
}

void SemiGlobalMatching::aggregateCost(int ***C) {
  std::cout << "Setting border costs!" << std::endl;
  // set border costs
  for (int xp = 0; xp < width_; xp++) {
    for (int d = 0; d < max_disparity_ + 1; d++) {
      S_[0][xp][d] = C[0][xp][d];
      S_[height_ - 1][xp][d] = C[height_ - 1][xp][d];
    }
  }

  // can skip first and last column because ^
  for (int yp = 1; yp < height_ - 1; yp++) {
    for (int d = 0; d < max_disparity_ + 1; d++) {
      S_[yp][0][d] = C[yp][0][d];
      S_[yp][width_ - 1][d] = C[yp][width_ - 1][d];
    }
  }

  std::cout << "Starting cost aggregation!" << std::endl;

  const int N_PATHS = 16;
  
  // traverse paths and aggregate costs
  for (int xp = 0; xp < width_; xp++) {
    for (int i = 0; i < N_PATHS; i++) {
      traversePath(xp, 0, paths[i], C);
      traversePath(xp, height_ - 1, paths[i], C);
    }
  }

  // can skip first and last column because ^
  for (int yp = 1; yp < height_ - 1; yp++) {
    for (int i = 0; i < N_PATHS; i++) {
      traversePath(0, yp, paths[i], C);
      traversePath(width_ - 1, yp, paths[i], C);
    }
  }
}

int** SemiGlobalMatching::calculateDisparities() {
  std::cout << "Calculating correspondence costs!" << std::endl;
  int ***cost = correspondence_->calculateCost();

  std::cout << "Aggregating costs!" << std::endl;
  aggregateCost(cost);

  // use aggregated costs instead of correspondence costs
  storeDisparities(S_);

  return disparity_map_;
}
