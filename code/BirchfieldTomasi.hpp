#ifndef BIRCHFIELD_TOMASSI_HPP
#define BIRCHFIELD_TOMASSI_HPP

#include "Correspondence.hpp"

class BirchfieldTomasi : public Correspondence {
private:
  int Cbt(int x, int y, int d, image &left_image, image &right_image);
public:
  using Correspondence::Correspondence;
  virtual int calculate(int x, int y, int d) override;
};

#endif
