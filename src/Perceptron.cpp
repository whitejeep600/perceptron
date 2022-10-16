#include <utility>

#include "perceptron.h"

Perceptron::Perceptron(Hyperplane h, bool recognizes_positive_side):
h(std::move(h)),
recognizes_positive_side(recognizes_positive_side)
{}

bool Perceptron::recognizes(const Pattern &p) {
    bool on_positive_side = h.on_positive_side(p.image.to_algebraic_vector());
    return on_positive_side == recognizes_positive_side;
}

