#ifndef CORRESPONDENCE_HPP
#define CORRESPONDENCE_HPP

#include <png++/png.hpp>
#include "predefs.hpp"

class Correspondence {
protected:
  image &left_image_;
  image &right_image_;
  int width_;
  int height_;
  int whalf_;
  int max_disparity_;
  int ***cost_;
  bool calculated_;
  
public:
  Correspondence(image &left_image, image &right_image, int window, int max_disparity);

  virtual ~Correspondence();

  virtual int calculate(int x, int y, int d) = 0;
  
  int*** calculateCost();

  int*** getCost();

  int getWidth() const;

  int getHeight() const;

  int getMaxDisparity() const;
};

#endif
