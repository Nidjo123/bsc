#include "Correspondence.hpp"

Correspondence::Correspondence(image &left_image, image &right_image, int window, int max_disparity)
  : left_image_(left_image), right_image_(right_image), width_(left_image.get_width()), height_(left_image.get_height()),  whalf_(window / 2), max_disparity_(max_disparity)  {
  cost_ = new int**[height_];

  for (int i = 0; i < height_; i++) {
    cost_[i] = new int*[width_];

    for (int j = 0; j < width_; j++) {
      cost_[i][j] = new int[max_disparity_ + 1];

      for (int k = 0; k < max_disparity_; k++) {
	cost_[i][j][k] = 999999999;
      }
    }
  }
}

Correspondence::~Correspondence() {
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      delete [] cost_[i][j];
    }
    delete [] cost_[i];
  }

  delete [] cost_;
}

int*** Correspondence::calculateCost() {
  for (int y = whalf_; y < height_ - whalf_; y++) {
    std::cout << (y / (float) (height_ - 2 * whalf_) * 100.f) << "%" << std::endl;
    for (int x = whalf_; x < width_ - whalf_; x++) {
      #pragma omp parallel for
      for (int d = 0; d <= max_disparity_; d++) {
	cost_[y][x][d] = calculate(x, y, d);
      }
    }
  }

  return cost_;
}

int*** Correspondence::getCost() {
  return cost_;
}

int Correspondence::getWidth() const {
  return width_;
}

int Correspondence::getHeight() const {
  return height_;
}

int Correspondence::getMaxDisparity() const {
  return max_disparity_;
}
