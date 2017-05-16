#ifndef SEMI_GLOBAL_MATCHING_HPP
#define SEMI_GLOBAL_MATCHING_HPP

#include "Correspondence.hpp"

class SemiGlobalMatching {
private:
  Correspondence &correspondence_;
  image &left_image_;
  image &right_image_;
  int width_;
  int height_;
  int max_disparity_;
  int **aggregateCost_;
  int **disparity_map_;

  void aggregateCost();
  
public:
  SemiGlobalMatching(image &left, image &right, int max_disparity);
  virtual ~SemiGlobalMatching();
  
  int** calculateDisparities();
};

#endif
