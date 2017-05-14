#include "Census.hpp"
#include <bitset>

typedef std::bitset<MAX_WINDOW_SIZE * MAX_WINDOW_SIZE> bitset;

int Census::calculate(int x, int y, int d) {
  bitset bits_left, bits_right;

  int current_bit = 0;
  
  for (int py = y - whalf_; py <= y + whalf_; py++) {
    for (int px = x - whalf_; px <= x + whalf_; px++) {
      bits_right[current_bit] = right_image_[py][px - d] < right_image_[y][x - d];
      bits_left[current_bit] = left_image_[py][px] < left_image_[y][x];
      current_bit++;
    }
  }

  return (bits_left ^ bits_right).count();
}
