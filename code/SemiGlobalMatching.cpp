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
}

bool SemiGlobalMatching::isBorderPixel(int x, int y) const {
  return x == 0 || y == 0 || x == width_ - 1 || y == height_ - 1;
}

void SemiGlobalMatching::traversePath(int sx, int sy, const Path &path, int ***C) {
  if (sx < 0 || sy < 0 || sx >= width_ || sy >= height_ || !isBorderPixel(sx, sy)) return;

  int x = sx + path.dx;
  int y = sy + path.dy;
  int prev_min_Lr = S[sy][sx][0];
  int min_d = -2;
  
  for (; x > 0 && y > 0 && x < width_ - 1 && y < height_ - 1; x += path.dx, y += path.dy) {
    int curr_Lr = 0;
    int curr_min_Lr = 9999999;
    int curr_min_d = 0;
    
    for (int d = 0; d < max_disparity_ + 1; d++) {
      curr_Lr = C[y][x][d] + prev_min_Lr;

      if (std::abs(min_d - d) == 1) {
	curr_Lr += P1;
      } else if (min_d != d && min_d >= 0) {
	curr_Lr += P2;
      }

      if (curr_min_Lr > curr_Lr) {
	curr_min_Lr = curr_Lr;
	curr_min_d = d;
      }

      S[y][x][d] += curr_Lr;
    }

    min_d = curr_min_d;
    prev_min_Lr = curr_min_Lr;
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
