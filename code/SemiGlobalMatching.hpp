#ifndef SEMI_GLOBAL_MATCHING_HPP
#define SEMI_GLOBAL_MATCHING_HPP

#include "Matching.hpp"

struct Path;

class SemiGlobalMatching : public Matching {
private:
  const int P1_;
  const int P2_;
  int ***S_;
  int *L_[2];

  bool isBorderPixel(int x, int y) const;
  void traversePath(int sx, int sy, const Path &path, int ***C);
  void aggregateCost(int ***C);
  
public:
  SemiGlobalMatching(Correspondence *correspondence, int P1, int P2);
  virtual ~SemiGlobalMatching();
  
  virtual int** calculateDisparities() override;
};

const int MAX_PATHS = 16;

#endif
