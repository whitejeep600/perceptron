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
    auto original_count = dataset.howmany_of_label(l);
    while(dataset.howmany_of_label(l) >= original_count / 20){
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
    auto neurons = create_neurons(l, dataset);
    double best_F1 = 0;
    double F1;
    uint32_t best_treshold = 0;
    for(uint32_t treshold = 0; treshold <= neurons.size(); ++treshold){
        F1 = PerceptronNetwork(neurons, treshold).test_on_dataset(dataset, l, false);
        cout << "achieved F1=" << F1 << " for treshold " << treshold << "\n";
        if(F1 > best_F1){
            best_F1 = F1;
            best_treshold = treshold;
        }
    }
    return PerceptronNetwork{neurons, best_treshold};
}

bool PerceptronNetwork::recognizes(const Pattern &pattern) {
    return this->howmany_recognize(pattern) >= this->treshold;
}

uint32_t PerceptronNetwork::howmany_recognize(const Pattern& pattern){
    uint32_t howmany = 0;
    for(auto& perceptron: this->perceptrons){
        if(perceptron.recognizes(pattern)) ++howmany;
    }
    return howmany;
}

// todo moze to do utils
// todo kolejnosc wszystkich funkcji etc plikach
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

double PerceptronNetwork::test_on_dataset(const Dataset& dataset, label l, bool print_details) {
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
    if (print_details) {
        cout << "dataset size: " << all << ".\n";
        cout << "true positives: " << true_positives << ", false positives: " << false_positives;
        cout << ", true negatives: " << true_negatives << ", false negatives: " << false_negatives << ".\n";
        cout << "precision: " << precision << ", recall: " << recall << ".\n";
        cout << "F1 score: " << F1 << ".\n";
    }
    return F1;
}
