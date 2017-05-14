#ifndef CENSUS_HPP
#define CENSUS_HPP

#include "Correspondence.hpp"

class Census : public Correspondence {
public:
  // inherit constructor
  using Correspondence::Correspondence;

  virtual int calculate(int x, int y, int d);
};

#endif
