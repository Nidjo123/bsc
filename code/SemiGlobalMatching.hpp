#ifndef SEMI_GLOBAL_MATCHING_HPP
#define SEMI_GLOBAL_MATCHING_HPP

#include "Correspondence.hpp"

struct Path;

class SemiGlobalMatching {
private:
  Correspondence &correspondence_;
  int width_;
  int height_;
  int max_disparity_;
  int ***S;
  int *L[2];
  int **disparity_map_;

  bool isBorderPixel(int x, int y) const;
  void traversePath(int sx, int sy, const Path &path, int ***C);
  void aggregateCost(int ***C);
  
public:
  SemiGlobalMatching(Correspondence *correspondence);
  virtual ~SemiGlobalMatching();
  
  int** calculateDisparities();
};

const int MAX_PATHS = 16;

#endif
