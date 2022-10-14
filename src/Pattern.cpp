#include "perceptron.h"

Pattern::Pattern(const Image&  image, label l):
image(image),
l(l),
h({}, 0)
{}

bool Pattern::operator==(const Pattern& that) const {
    return this->image == that.image and this->l == that.l;
}
