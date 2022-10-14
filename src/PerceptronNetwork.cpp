#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include "perceptron.h"
#include "hyperplane.h"
#include "utils.h"
// I ended up using this library for matrix inversions.
// Surprisingly no more 'normal' library is available

using namespace std;

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

// Create a vector of perceptrons to recognize label l based on the dataset. If any of the
// resulting perceptrons light up for a pattern, it will be assigned the label l.
vector<Perceptron> create_to_recognize(label l, Dataset dataset){
    // passing dataset by value because it's convenient to remove stuff from it while learning,
    // but we don't want this to affect the original dataset
    vector<Perceptron> result;
    dataset.preprocess(l);
    vector<Pattern> same_side_and_label;
    vector<Pattern> biggest_same_side_and_label;
    Hyperplane associated_with_biggest;
    while(dataset.contains_label(l)){
        for(const auto& pattern: dataset.patterns){
            if(pattern.l == l){
                same_side_and_label = get_all_with_same_side_and_label(pattern, *(pattern.h), dataset);
                if(same_side_and_label.size() > biggest_same_side_and_label.size()){
                    biggest_same_side_and_label = same_side_and_label;
                    associated_with_biggest = *(pattern.h);
                }
            }
        }
        Pattern nearest = get_nearest(biggest_same_side_and_label, associated_with_biggest);
        associated_with_biggest.move_halfway_to_point(nearest.image.to_algebraic_vector());
        result.emplace_back(associated_with_biggest,
                            associated_with_biggest.on_positive_side(nearest.image.to_algebraic_vector()));
        dataset.remove_patterns(biggest_same_side_and_label);
    }
    return result;
}
