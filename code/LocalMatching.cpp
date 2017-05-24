#include "LocalMatching.hpp"
#include <limits>
#include <cmath>

int** LocalMatching::calculateDisparities() {
  int ***cost = correspondence_->calculateCost();
  
  storeDisparities(cost);
  
  return disparity_map_;
}

