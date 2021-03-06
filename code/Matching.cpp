#include "Matching.hpp"

Matching::Matching(Correspondence *correspondence)
  : correspondence_(correspondence), width_(correspondence->getWidth()),
    height_(correspondence->getHeight()), max_disparity_(correspondence->getMaxDisparity()) {
  disparity_map_ = new int*[height_];

  for (int y = 0; y < height_; y++) {
    disparity_map_[y] = new int[width_];

    for (int x = 0; x < width_; x++) {
      disparity_map_[y][x] = 0;
    }
  }
}

Matching::~Matching() {
  for (int y = 0; y < height_; y++) {
    delete [] disparity_map_[y];
  }

  delete [] disparity_map_;
}

int** Matching::getDisparityMap() {
  return disparity_map_;
}

void Matching::storeDisparities(int ***cost) {
  // find disparities with minimum costs
  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      int min_cost = std::numeric_limits<int>::max();
      for (int d = 0; d <= max_disparity_; d++) {
	if (cost[y][x][d] < min_cost) {
	  min_cost = cost[y][x][d];
	  disparity_map_[y][x] = d;
	}
      }
    }
  }
}
