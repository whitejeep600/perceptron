#include <valarray>
#include <cassert>
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

double average_of_ith_column(const Matrix& matrix, uint32_t column){
    double sum = 0;
    for(uint32_t i = 0; i < matrix.size(); ++i){
        sum += matrix[i][column];
    }
    return sum / (double) matrix.size();
}

Hyperplane lead_through(const vector<Pattern>& patterns, const Pattern& target){
    auto matrix = Matrix (IMAGE_SIZE);
    uint32_t row = 0;
    assert(patterns.size() == IMAGE_SIZE);
    for(const auto& p: patterns){
        matrix[row] = std::vector<double>(IMAGE_SIZE);
        for(uint32_t column = 0; column < IMAGE_SIZE; ++column){
            matrix[row][column] = p.image.pixels[column];
        }
        ++row;
    }
    vector<double> res;
    bool at_least_one_nonzero = false;
    for(uint32_t i = 0; i < matrix.size(); ++i){
        if(column_has_only_zeroes(matrix, i)){
            res.push_back(target.image.pixels[i]);
            if(target.image.pixels[i] != 0){
                at_least_one_nonzero = true;
            }
        }
        else{
            res.push_back(0.0);
        }
    }
    if(at_least_one_nonzero){
        return {res, 0.0};
    }
    else{
        res.clear();
        for(uint32_t i = 0; i < matrix.size(); ++i){
            res.push_back(target.image.pixels[i] - average_of_ith_column(matrix, i));
        }
        double mean_pattern_dot_product = 0;
        for(auto& p: patterns){
            mean_pattern_dot_product += dot_product(p.image.to_algebraic_vector(), res);
        }
        mean_pattern_dot_product /= (double) patterns.size();
        double target_dot_product = dot_product(target.image.to_algebraic_vector(), res);
        assert(target_dot_product > mean_pattern_dot_product);
        return {res, mean_pattern_dot_product +
                     0.2 * (target_dot_product - mean_pattern_dot_product)};
                     // ^ this can be adjusted, maybe should be 0 bc the
                     // hyperplane is moved halfway to the target pattern
                     // anyway later on
                     // zasadniczo można ten współczynnik dobierać
                     // dynamicznie za każdym razem, np wypróbowywać
                     // kilka wartości i patrzeć która działa najlepiej
    }
}
