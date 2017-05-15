#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "predefs.hpp"

class Evaluator {
public:
  static double error(image &left_truth, image &right_truth, image &test);
};

#endif
