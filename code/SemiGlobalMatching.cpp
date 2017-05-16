#include "SemiGlobalMatching.hpp"

const int P1 = 1;
const int P2 = 20;

SemiGlobalMatching::SemiGlobalMatching(Correspondence &corr, image &left, image &right, int max_disparity)
  : correspondence_(corr), left_image_(left), right_image_(right), width_(left.get_width()), height_(left.get_height()), max_disparity_(max_disparity) {
  disparity_map_ = new int*[height_];

  for (int i = 0; i < height_; i++) {
    disparity_map_[i] = new int[width_];
  }

  aggregateCost_ = new int*[height_];

  for (int i = 0; i < height_; i++) {
    aggregateCost_[i] = new int[width_];
  }
}

int SemiGlobalMatching::calculateDisparities() {
  int ***cost = correspondence.calculateCost();

  
}
