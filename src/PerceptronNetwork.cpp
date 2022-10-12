#include <iostream>
#include "perceptron.h"
#include "hyperplane.h"
#include "eigen/Eigen/Dense"

using namespace std;

set<Pattern> get_nearest_with_different_label(const Pattern& pattern, uint32_t how_many){
    return {};
    // todo
}

// Let the plane equation be sum(alpha_n * x_n = 1
Hyperplane lead_through(const set<Pattern>& patterns){
    auto m = Eigen::Matrix<double, IMAGE_HEIGHT, IMAGE_WIDTH>(IMAGE_HEIGHT, IMAGE_WIDTH);
    assert(patterns.size() == IMAGE_HEIGHT);
    return {};
    // todo
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