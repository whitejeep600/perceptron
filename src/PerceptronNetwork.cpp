#include <iostream>
#include <utility>
#include <algorithm>
#include <numeric>
#include <map>
#include "perceptron.h"
#include "hyperplane.h"

using namespace std;

vector<Pattern> get_all_with_same_side_and_label(const Pattern& target, const Hyperplane& h, Dataset& dataset){
    vector<Pattern> result;
    for(Pattern& p: dataset.patterns){
        if(p.l == target.l and h.on_same_side(target.image.to_algebraic_vector(),
                                              p.image.to_algebraic_vector())){
            result.push_back(p);
        }
    }
    return result;
}

vector<Pattern> get_all_with_same_side_and_different_label(const Pattern& target,
                                                           const Hyperplane& h,
                                                           Dataset& dataset){
    vector<Pattern> result;
    for(Pattern& p: dataset.patterns){
        if(p.l != target.l and h.on_same_side(target.image.to_algebraic_vector(),
                                              p.image.to_algebraic_vector())){
            result.push_back(p);
        }
    }
    return result;
}

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

vector<Perceptron> create_neurons(label l, Dataset dataset){
    // passing dataset by value because it's convenient to remove stuff from it while learning,
    // but we don't want this to affect the original dataset
    vector<Perceptron> result;
    vector<Pattern> same_side_and_label;
    vector<Pattern> same_side_and_different_label;
    vector<Pattern> biggest_same_side_and_label;
    Hyperplane associated_with_biggest;
    while(dataset.contains_label(l)){
        for(const auto& pattern: dataset.patterns){
            if(pattern.l == l){
                same_side_and_label = get_all_with_same_side_and_label(pattern, *(pattern.h), dataset);
                same_side_and_different_label =
                        get_all_with_same_side_and_different_label(pattern, *(pattern.h), dataset);
                if(same_side_and_label.size() > biggest_same_side_and_label.size()) {
                    biggest_same_side_and_label = same_side_and_label;
                    associated_with_biggest = *(pattern.h);
                    cout << "selecting neuron with tp " << same_side_and_label.size()
                         << ", fp " << same_side_and_different_label.size() << "\n";
                }
            }
        }
        Pattern nearest = get_nearest(biggest_same_side_and_label, associated_with_biggest);
        associated_with_biggest.move_halfway_to_point(nearest.image.to_algebraic_vector());
        result.emplace_back(associated_with_biggest,
                            associated_with_biggest.on_positive_side(nearest.image.to_algebraic_vector()));
        dataset.remove_patterns(biggest_same_side_and_label);
        cout << "removing " << biggest_same_side_and_label.size() << "patterns\n" ;
        biggest_same_side_and_label.clear();
    }
    return result;
}

// Create a vector of perceptrons to recognize label l based on the dataset. If any of the
// resulting perceptrons light up for a pattern, it will be assigned the label l.
PerceptronNetwork create_to_recognize(label l, Dataset& dataset, bool from_preprocessed){
    if(from_preprocessed){
        dataset.preprocess_from_dump(l);
    }
    else{
        dataset.preprocess(l, true);
    }
    auto result = create_neurons(l, dataset);
    return PerceptronNetwork{result, 0};
}

bool PerceptronNetwork::recognizes(const Pattern &pattern) {
    for(auto& perceptron: perceptrons){
        if(perceptron.recognizes(pattern)) return true;
    }
    return false;
}

uint32_t PerceptronNetwork::howmany_recognize(const Pattern& pattern){
    uint32_t howmany = 0;
    for(auto& perceptron: perceptrons){
        if(perceptron.recognizes(pattern)) ++howmany;
    }
    return howmany;
}

template<class T>
map<T, uint32_t> get_counts(const vector<T>& vec){
    auto values = set<T>();
    for(const auto howmany: vec){
        values.insert(howmany);
    }
    auto counts = std::map<T, uint32_t>();
    for(const auto value: values){
        counts.insert({value,
                          count(vec.begin(), vec.end(), value)});
    }
    return counts;
}

void PerceptronNetwork::test_on_dataset(const Dataset& dataset, label l) {
    uint32_t all = dataset.patterns.size();
    uint32_t true_positives = 0;
    uint32_t false_positives = 0;
    uint32_t true_negatives = 0;
    uint32_t false_negatives = 0;
    vector<uint32_t> howmany_recognize_tp;
    vector<uint32_t> howmany_recognize_fp;
    for(auto& pattern: dataset.patterns){
        if(pattern.l == l){
            if(this->recognizes(pattern)){
                ++true_positives;
                howmany_recognize_tp.push_back(howmany_recognize(pattern));
            }
            else{
                ++false_negatives;
            }
        }
        else{
            if(this->recognizes(pattern)){
                ++false_positives;
                howmany_recognize_fp.push_back(howmany_recognize(pattern));
            }
            else{
                ++true_negatives;
            }
        }
    }
    double precision = ((double) true_positives) / (true_positives + false_positives);
    double recall = ((double) true_positives) / (true_positives + false_negatives);
    double F1 = ((double) 2.0) * precision * recall / (precision + recall);
    cout << "dataset size: " << all << ".\n";
    cout << "true positives: " << true_positives << ", false positives: " << false_positives;
    cout << ", true negatives: " << true_negatives << ", false negatives: " << false_negatives << ".\n";
    cout << "precision: " << precision << ", recall: " << recall << ".\n";
    cout << "F1 score: " << F1 << ".\n";
    cout << "average neurons active for tp: " << reduce(howmany_recognize_tp.begin(), howmany_recognize_tp.end()) / (double) howmany_recognize_tp.size() << ".\n";
    cout << "average neurons active for fp: " << reduce(howmany_recognize_fp.begin(), howmany_recognize_fp.end()) / (double) howmany_recognize_fp.size() << ".\n";
    auto tp_counts = get_counts(howmany_recognize_tp);
    auto fp_counts = get_counts(howmany_recognize_fp);
    cout << "precise counts of neurons recognizing true positive values:\n";
    for(const auto count: tp_counts){
        cout << count.second << " patterns recognized by exactly " << count.first << " neurons.\n";
    }
    cout << "precise counts of neurons recognizing false positive values:\n";
    for(const auto count: fp_counts){
        cout << count.second << " patterns recognized by exactly " << count.first << " neurons.\n";
    }
}
