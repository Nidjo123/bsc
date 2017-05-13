#ifndef LOCAL_MATCHING_HPP
#define LOCAL_MATCHING_HPP

#include "Correspondence.hpp"

class LocalMatching {
private:
  Correspondence *correspondence_;
  int **disparity_map_;
  int width_;
  int height_;
  int max_disparity_;

public:
  LocalMatching(Correspondence *correspondence);

  virtual ~LocalMatching();

  int** calculateDisparities();

  int** getDisparityMap();
};

#endif
