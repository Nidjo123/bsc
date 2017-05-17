#include "SemiGlobalMatching.hpp"
#include <cmath>

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

const int P1 = 3;
const int P2 = 20;

SemiGlobalMatching::SemiGlobalMatching(Correspondence *correspondence)
  : correspondence_(*correspondence), width_(correspondence_.getWidth()), height_(correspondence_.getHeight()), max_disparity_(correspondence_.getMaxDisparity()) {
  disparity_map_ = new int*[height_];

  for (int i = 0; i < height_; i++) {
    disparity_map_[i] = new int[width_];
  }

  S = new int**[height_];
  
  for (int i = 0; i < height_; i++) {
    S[i] = new int*[width_];

    for (int j = 0; j < width_; j++) {
      S[i][j] = new int[max_disparity_ + 1];
      for (int k = 0; k < max_disparity_ + 1; k++) {
	S[i][j][k] = 0;
      }
    }
  }

  L[0] = new int[max_disparity_ + 1];
  L[1] = new int[max_disparity_ + 1];
}

SemiGlobalMatching::~SemiGlobalMatching() {
  // delete S and disparity map

  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      delete [] S[y][x];
    }
    delete [] S[y];

    delete [] disparity_map_[y];
  }

  delete [] S;

  delete [] disparity_map_;

  delete [] L[0];
  delete [] L[1];
}

bool SemiGlobalMatching::isBorderPixel(int x, int y) const {
  return x == 0 || y == 0 || x == width_ - 1 || y == height_ - 1;
}

void SemiGlobalMatching::traversePath(int sx, int sy, const Path &path, int ***C) {
  if (sx < 0 || sy < 0 || sx >= width_ || sy >= height_ || !isBorderPixel(sx, sy)) return;

  for (int d = 0; d < max_disparity_ + 1; d++) {
    L[0][d] = C[sy][sx][d];
  }

  int x = sx + path.dx;
  int y = sy + path.dy;
  int min_prev_Lr = C[sy][sx][0];
  
  for (; x > 0 && y > 0 && x < width_ - 1 && y < height_ - 1; x += path.dx, y += path.dy) {
    int min_Lr = 999999999;
    
    for (int d = 0; d < max_disparity_ + 1; d++) {
      int Lr = 999999999;

      for (int dp = 0; dp < max_disparity_ + 1; dp++) {
	if (d == dp && Lr > L[0][dp]) {
	  Lr = L[0][dp];
	} else if (std::abs(d - dp) == 1 && Lr > L[0][dp] + P1) {
	  Lr = L[0][dp] + P1;
	} else if (Lr > L[0][dp] + P2) {
	  Lr = L[0][dp] + P2;
	}
      }

      S[y][x][d] += (L[1][d] = C[y][x][d] + Lr - min_prev_Lr);
      min_Lr = std::min(min_Lr, L[1][d]);
    }

    min_prev_Lr = min_Lr;

    for (int i = 0; i < max_disparity_ + 1; i++) {
      L[0][i] = L[1][i];
    }
  }
}

void SemiGlobalMatching::aggregateCost(int ***C) {
  std::cout << "Setting border costs!" << std::endl;
  // set border costs
  for (int xp = 0; xp < width_; xp++) {
    for (int d = 0; d < max_disparity_ + 1; d++) {
      S[0][xp][d] = C[0][xp][d];
      S[height_ - 1][xp][d] = C[height_ - 1][xp][d];
    }
  }

  // can skip first and last column because ^
  for (int yp = 1; yp < height_ - 1; yp++) {
    for (int d = 0; d < max_disparity_ + 1; d++) {
      S[yp][0][d] = C[yp][0][d];
      S[yp][width_ - 1][d] = C[yp][width_ - 1][d];
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
  int ***cost = correspondence_.calculateCost();

  aggregateCost(cost);

  // fill disparity map
  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      int min = S[y][x][0];
      int min_d = 0;
      for (int d = 1; d < max_disparity_ + 1; d++) {
	if (S[y][x][d] < min) {
	  min = S[y][x][d];
	  min_d = d;
	}
      }

      disparity_map_[y][x] = min_d;
    }
  }

  return disparity_map_;
}
