#include "BirchfieldTomasi.hpp"
#include <cmath>

#define MAX_COST 99999999

int BirchfieldTomasi::Cbt(int leftX, int rightX, int y, image &left_image, image &right_image) {
  unsigned leftVal, midVal, rightVal, leftMidVal;

  if (rightX < 0) return MAX_COST;

  midVal = right_image[y][rightX];
  leftMidVal = left_image[y][leftX];
  
  if (rightX > 0) {
    // left position exists
    leftVal = right_image[y][rightX - 1];
  } else {
    leftVal = midVal;
  }

  if (rightX < width_ - 1) {
    // right position exists
    rightVal = right_image[y][rightX + 1];
  } else {
    rightVal = midVal;
  }
  
  
  const double Ip = (midVal + rightVal) / 2.0;
  const double Im = (midVal + leftVal) / 2.0;

  const double Imin = std::min(std::min(Im, Ip), (double) midVal);
  const double Imax = std::max(std::max(Im, Ip), (double) midVal);

  return std::max(0.0, std::max(leftMidVal - Imax, Imin - leftMidVal));
}

int BirchfieldTomasi::calculate(int x, int y, int d) {
  return std::min(Cbt(x, x - d, y, left_image_, right_image_),
		  Cbt(x - d, x, y, right_image_, left_image_));
}
