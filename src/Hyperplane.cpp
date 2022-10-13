#include <valarray>
#include "hyperplane.h"
#include "utils.h"

void Hyperplane::move_halfway_to_point(const vector<double>&  point){
    // The vector leading from the hyperplane c'x=b to point p is c * (dot(c,p)-b) / dot(c, c).
    // We can find this vector and translate the hyperplane by this vector.
    auto scalar = 0.5 *
            (dot_product(coefficients_vector, point) - constant_term) /
            dot_product(coefficients_vector, coefficients_vector);
    auto translation_vector = mul_vector_by_scalar(coefficients_vector, scalar);
    this->translate_by_vector(translation_vector);
}

Hyperplane::Hyperplane(const vector<double>& coefficients_vector, double constant_term)
:
coefficients_vector(coefficients_vector),
constant_term(constant_term)
{}

Hyperplane::Hyperplane()
:
coefficients_vector(),
constant_term(0)
{}

bool Hyperplane::on_positive_side(const vector<double>& vec) const {
    return dot_product(coefficients_vector, vec) > constant_term;
}

bool Hyperplane::on_same_side(const vector<double> &vec1, const vector<double> &vec2) const {
    bool on_positive1 = this->on_positive_side(vec1);
    bool on_positive2 = this->on_positive_side(vec2);
    return on_positive1 == on_positive2;
}

double Hyperplane::distance_to_point(const vector<double> &vec) const {
    return abs(dot_product(coefficients_vector, vec) + constant_term) /
    euclidean_norm(coefficients_vector);
}

// To translate the hyperplane c'x = b, we can add c't to b (I don't know if this is common
// knowledge to be honest, I proved this on paper)
void Hyperplane::translate_by_vector(const vector<double> &translation_vector) {
    constant_term += dot_product(coefficients_vector, translation_vector);
}


