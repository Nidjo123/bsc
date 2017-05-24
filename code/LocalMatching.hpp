#ifndef LOCAL_MATCHING_HPP
#define LOCAL_MATCHING_HPP

#include "Matching.hpp"

class LocalMatching : public Matching {
public:
  // inherit constructor
  using Matching::Matching;

  virtual int** calculateDisparities() override;
};

#endif
