#include "ZSAD.hpp"
#include <cmath>

int ZSAD::calculate(int x, int y, int d) {
  int cost = 0;
  int avg_left;
  int avg_right;

  for (int px = x - whalf_; px <= x + whalf_; px++) {
    for (int py = y - whalf_; py <= y + whalf_; py++) {
      avg_left += left_image_[py][px];
      avg_right += right_image_[py][px - d];
    }
  }

  const int area = pow(whalf_ * 2 + 1, 2);
  avg_left /= (double) area;
  avg_right /= (double) area;

  for (int px = x - whalf_; px <= x + whalf_; px++) {
    for (int py = y - whalf_; py <= y + whalf_; py++) {
      cost += std::abs(right_image_[py][px - d] - avg_right - left_image_[py][px] + avg_left);
    }
  }

  return cost;
}
