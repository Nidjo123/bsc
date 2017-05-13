#ifndef ZSAD_HPP
#define ZSAD_HPP

#include "Correspondence.hpp"

class ZSAD : public Correspondence {
public:
  // inherit constructor
  using Correspondence::Correspondence;
  
  virtual int calculate(int x, int y, int d) override;
};

#endif
