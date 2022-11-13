#include <utility>

#include "perceptron.h"

Perceptron::Perceptron(Hyperplane h, bool recognizes_positive_side):
recognizes_positive_side(recognizes_positive_side),
h(std::move(h))
{}

bool Perceptron::recognizes(const Pattern &p) const {
    bool on_positive_side = h.on_positive_side(p.image.to_algebraic_vector());
    return on_positive_side == recognizes_positive_side;
}

