#include <iostream>
#include "perceptron.h"
#include "hyperplane.h"
#include "eigen/Eigen/Dense"
// I ended up using this library for matrix inversions.
// Surprisingly no more 'normal' library is available

using namespace std;

set<Pattern> get_nearest_with_different_label(const Pattern& pattern, uint32_t how_many){
    return {};
    // todo
}

// Let the plane equation in n dimensions be sum(c_n * x_n = 1) (this is assuming that
// zero does not belong to the plane but that is extremely unlikely to happen). Then let c
// be the vector of c_i. We are given a set of vectors belonging to that plane. If we
// assemble these vectors into matrix A so that each vector becomes a row, c satisfies
// Ax = 1 (vector of ones). Then x = A^{-1} * 1.
Hyperplane lead_through(const set<Pattern>& patterns){
    auto A = Eigen::Matrix<double, IMAGE_HEIGHT, IMAGE_WIDTH>();
    uint32_t row = 0;
    assert(patterns.size() == IMAGE_HEIGHT);
    for(auto p: patterns){
        for(uint32_t column = 0; column < IMAGE_WIDTH; ++column){
            A(row, column) = p.image.pixels[column];
        }
        ++row;
    }
    auto A_inverted = A.inverse();
    auto ones = Eigen::Matrix<double, IMAGE_HEIGHT, 1>();
    for(uint32_t vector_row = 0; vector_row < IMAGE_HEIGHT; ++vector_row){
        ones(vector_row, 1) = 1;
    }
    Eigen::Matrix<double, IMAGE_HEIGHT, 1> coefficients_eigen_vector = A_inverted * ones;
    vector<double> coefficients_vector;
    for(uint32_t vector_row = 0; vector_row < IMAGE_HEIGHT; ++vector_row){
        coefficients_vector.push_back(coefficients_eigen_vector(vector_row, 1));
    }
    return {coefficients_vector, 1};
}

set<Pattern> get_with_same_side_and_label(const Pattern& p, const Hyperplane h){
    return {};
    // todo
}

Pattern get_nearest(const set<Pattern>& patterns, const Hyperplane h){
    // todo
}

// Create a vector of perceptrons to recognize label l based on the dataset. If any of the
// resulting perceptrons light up for a pattern, it will be assigned the label l.
vector<Perceptron> create_to_recognize(label l, Dataset& dataset){
    vector<Perceptron> result;
    Hyperplane h;
    set<Pattern> nearest_different_label;
    set<Pattern> same_side_and_label;
    set<Pattern> biggest_same_side_and_label;
    Hyperplane associated_with_biggest;
    while(dataset.contains_label(l)){
        for(auto pattern: dataset.patterns){
            if(pattern.l == l){
                nearest_different_label = get_nearest_with_different_label(pattern, 784);
                h = lead_through(nearest_different_label);
                same_side_and_label = get_with_same_side_and_label(pattern, h);
                if(same_side_and_label.size() > biggest_same_side_and_label.size()){
                    biggest_same_side_and_label = same_side_and_label;
                    associated_with_biggest = h;
                }
            }
        }
        Pattern nearest = get_nearest(biggest_same_side_and_label, associated_with_biggest);
        associated_with_biggest.move_halfway_to_point(nearest.image.pixels);
        result.emplace_back(associated_with_biggest,
                            nearest.image.on_positive_side(associated_with_biggest));
        dataset.remove_patterns(biggest_same_side_and_label);
    }
}