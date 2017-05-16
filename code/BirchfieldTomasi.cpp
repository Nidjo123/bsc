#include "BirchfieldTomasi.hpp"
#include <cmath>

#define MAX_COST 99999999

int BirchfieldTomasi::Cbt(int x, int y, int d, image &left_image, image &right_image) {
  unsigned leftVal, midVal, rightVal, leftMidVal;
  const int xp = x - d;
  const int yp = y;

  if (xp < 0) return MAX_COST;

  midVal = right_image[yp][xp];
  leftMidVal = left_image[y][x];
  
  if (xp > 0) {
    // left position exists
    leftVal = right_image[yp][xp - 1];
  } else {
    leftVal = midVal;
  }

  if (xp < width_ - 1) {
    // right position exists
    rightVal = right_image[yp][xp + 1];
  } else {
    rightVal = midVal;
  }
  
  
  const double Ip = (midVal + rightVal) / 2.0;
  const double Im = (midVal + leftVal) / 2.0;

  const double Imin = std::min(std::min(Im, Ip), (double) right_image[yp][xp]);
  const double Imax = std::max(std::max(Im, Ip), (double) right_image[yp][xp]);

  return std::max(0.0, std::max(leftMidVal - Imax, Imin - leftMidVal));
}

int BirchfieldTomasi::calculate(int x, int y, int d) {
  return std::min(Cbt(x, y, d, right_image_, left_image_),
		  Cbt(x, y, d, left_image_, right_image_));
}
