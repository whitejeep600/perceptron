#include "hyperplane.h"
#include "utils.h"

//void Hyperplane::move_halfway_to_point(const uint8_t point[784]) {
//        // todo
//}

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


