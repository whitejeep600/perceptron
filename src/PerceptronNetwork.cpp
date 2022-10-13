#include <iostream>
#include <utility>
#include "perceptron.h"
#include "hyperplane.h"
#include "../eigen/Eigen/Dense"
// I ended up using this library for matrix inversions.
// Surprisingly no more 'normal' library is available

using namespace std;

vector<Pattern> get_nearest_with_different_label(const Pattern& pattern, uint32_t how_many, Dataset& dataset){
    struct key
    {
        Pattern target_pattern;
        explicit key(Pattern p): target_pattern(p){}
        bool operator() (const Pattern& pattern1, const Pattern& pattern2) const
        {
            return (pattern1.image.euclidean_distance_squared(target_pattern.image) <
                    pattern2.image.euclidean_distance_squared(target_pattern.image));
        }
    };
    auto sort_key = key(pattern);
    std::sort(dataset.patterns.begin(), dataset.patterns.end(), sort_key);
    auto result = vector<Pattern>();
    uint32_t found = 0;
    uint32_t i = 0;
    while(found < how_many){
        if(dataset.patterns[i].l != pattern.l){
            result.push_back(dataset.patterns[i]);
            ++found;
        }
        ++i;
    }
    return result;
}

// Let the plane equation in n dimensions be sum(c_n * x_n = 1) (this is assuming that
// zero does not belong to the plane but that is extremely unlikely to happen). Then let c
// be the vector of c_i. We are given a set of vectors belonging to that plane. If we
// assemble these vectors into matrix A so that each vector becomes a row, c satisfies
// Ax = 1 (vector of ones). Then x = A^{-1} * 1.
Hyperplane lead_through(const vector<Pattern>& patterns){
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
        ones(vector_row, 0) = 1;
    }
    Eigen::Matrix<double, IMAGE_HEIGHT, 1> coefficients_eigen_vector = A_inverted * ones;
    vector<double> coefficients_vector;
    for(uint32_t vector_row = 0; vector_row < IMAGE_HEIGHT; ++vector_row){
        coefficients_vector.push_back(coefficients_eigen_vector(vector_row, 0));
    }
    return {coefficients_vector, 1};
}

vector<Pattern> get_all_with_same_side_and_label(const Pattern& target, const Hyperplane& h, Dataset& dataset){
    vector<Pattern> result;
    for(Pattern p: dataset.patterns){
        if(p.l == target.l and h.on_same_side(target.image.to_algebraic_vector(),
                                              p.image.to_algebraic_vector())){
            result.push_back(p);
        }
    }
    return result;
}
//
Pattern get_nearest(vector<Pattern>& patterns, const Hyperplane& h){
    struct key
    {
        Hyperplane target_hyperplane;
        explicit key(Hyperplane h): target_hyperplane(std::move(h)){}
        bool operator() (const Pattern& pattern1, const Pattern& pattern2) const
        {
            return (target_hyperplane.distance_to_point(pattern1.image.to_algebraic_vector()) <
                    target_hyperplane.distance_to_point(pattern2.image.to_algebraic_vector()));
        }
    };
    auto sort_key = key(h);
    std::sort(patterns.begin(), patterns.end(), sort_key);
    return patterns[0];
}
//
//// Create a vector of perceptrons to recognize label l based on the dataset. If any of the
//// resulting perceptrons light up for a pattern, it will be assigned the label l.
//vector<Perceptron> create_to_recognize(label l, Dataset dataset){
//    vector<Perceptron> result;
//    Hyperplane h;
//    vector<Pattern> nearest_different_label;
//    vector<Pattern> same_side_and_label;
//    vector<Pattern> biggest_same_side_and_label;
//    Hyperplane associated_with_biggest;
//    while(dataset.contains_label(l)){
//        for(auto pattern: dataset.patterns){
//            if(pattern.l == l){
//                nearest_different_label = get_nearest_with_different_label(pattern, 784, dataset);
//                h = lead_through(nearest_different_label);
//                same_side_and_label = get_with_same_side_and_label(pattern, h, dataset);
//                if(same_side_and_label.size() > biggest_same_side_and_label.size()){
//                    biggest_same_side_and_label = same_side_and_label;
//                    associated_with_biggest = h;
//                }
//            }
//        }
//        Pattern nearest = get_nearest(biggest_same_side_and_label, associated_with_biggest);
//        associated_with_biggest.move_halfway_to_point(nearest.image.pixels);
//        result.emplace_back(associated_with_biggest,
//                            associated_with_biggest.on_positive_side(nearest.image.pixels));
//        dataset.remove_patterns(biggest_same_side_and_label);
//    }
//    return result;
//}
