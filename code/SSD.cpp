#include "SSD.hpp"

int SSD::calculate(int x, int y, int d) {
  int cost = 0;

  for (int px = x - whalf_; px <= x + whalf_; px++) {
    for (int py = y - whalf_; py <= y + whalf_; py++) {
      const int currCost = right_image_[py][px - d] - left_image_[py][px];
      cost += currCost * currCost;
    }
  }

  return cost;
}
