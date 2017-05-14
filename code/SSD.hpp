#ifndef SSD_HPP
#define SSD_HPP

#include "Correspondence.hpp"

class SSD : public Correspondence {
public:
  // inherit constructor
  using Correspondence::Correspondence;
  
  virtual int calculate(int x, int y, int d) override;
};

#endif
