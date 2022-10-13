#include <utility>

#include "perceptron.h"

Perceptron::Perceptron(Hyperplane h, bool recognizes_positive_side):
h(std::move(h)),
recognizes_positive_side(recognizes_positive_side)
{}

