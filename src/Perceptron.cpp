#include <utility>

#include "perceptron.h"

Perceptron::Perceptron(Hyperplane h, bool on_positive_side):
h(std::move(h)),
on_positive_side(on_positive_side)
{}

