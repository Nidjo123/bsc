#ifndef MATCHING_HPP
#define MATCHING_HPP

#include "Correspondence.hpp"

class Matching {
protected:
  Correspondence *correspondence_;
  int **disparity_map_;
  int width_;
  int height_;
  int max_disparity_;
  
public:
  Matching(Correspondence *corresp);

  virtual ~Matching();

  virtual int** calculateDisparities() = 0;

  int** getDisparityMap();

protected:
  void storeDisparities(int ***cost);
};

#endif
