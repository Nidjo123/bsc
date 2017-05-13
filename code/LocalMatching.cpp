#include "LocalMatching.hpp"
#include <limits>
#include <cmath>

LocalMatching::LocalMatching(Correspondence *correspondence)
  : correspondence_(correspondence), width_(correspondence->getWidth()),
    height_(correspondence->getHeight()), max_disparity_(correspondence->getMaxDisparity()) {
  disparity_map_ = new int*[height_];

  for (int y = 0; y < height_; y++) {
    disparity_map_[y] = new int[width_];
  }
}

LocalMatching::~LocalMatching() {
  for (int y = 0; y < height_; y++) {
      delete [] disparity_map_[y];
  }

  delete [] disparity_map_;
}

int** LocalMatching::calculateDisparities() {
  int ***cost = correspondence_->calculateCost();
  const float scale = 255.f / (max_disparity_ + 1);
  
  // find disparities with minimum costs
  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      int min_cost = std::numeric_limits<int>::max();

      #pragma omp parallel for
      for (int d = 0; d <= max_disparity_; d++) {
	if (cost[y][x][d] < min_cost) {
	  min_cost = cost[y][x][d];
	  disparity_map_[y][x] = std::round(d * scale);
	}
      }
    }
  }

  return disparity_map_;
}

int** LocalMatching::getDisparityMap() {
  return disparity_map_;
}
