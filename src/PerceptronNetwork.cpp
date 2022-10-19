#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <numeric>
#include "perceptron.h"
#include "hyperplane.h"
#include "utils.h"
// I ended up using this library for Matrix inversions.
// Surprisingly no more 'normal' library is available

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

// Create a vector of perceptrons to recognize label l based on the dataset. If any of the
// resulting perceptrons light up for a pattern, it will be assigned the label l.
PerceptronNetwork create_to_recognize(label l, Dataset dataset, bool from_preprocessed){
    // passing dataset by value because it's convenient to remove stuff from it while learning,
    // but we don't want this to affect the original dataset
    vector<Perceptron> result;
    if(from_preprocessed){
        dataset.preprocess_from_dump(l);
        // change this to dataset.preprocess(l, true) to dump the preprocessing results to file
    }
    else{
        //dataset.preprocess(l, false);
        dataset.preprocess(l, true);
    }
    vector<Pattern> same_side_and_label;
    vector<Pattern> same_side_and_different_label;
    Hyperplane best_hyperplane;
    vector<Pattern> selected_by_best_hyperplane;
    double precision;
    double best_precision;
    while(dataset.contains_label(l)){
        best_precision = 0;
        for(const auto& pattern: dataset.patterns){
            if(pattern.l == l){
                same_side_and_label = get_all_with_same_side_and_label(pattern, *(pattern.h), dataset);
                same_side_and_different_label =
                        get_all_with_same_side_and_different_label(pattern, *(pattern.h), dataset);
                // mozna tu zostaic greedy approach i zamienic kryterium - np precyzje priorytetyzowac
                // z jednej strony powinna byc priorytetyzowana precyzja. z drugiej strony jesli sie przesadzi,
                // to bedziemy moze wybierac neurony precyzyjnie rozpoznajace po parę labeli, i skończymy
                // z masą precycyjnych nerłonów, które razem będą jednak mało precyzyjne...
                // moze obczaic, ile neuronow srednio rozpoznaje prawdziwy/falszywy label i ustanowic jakis treshhold?
                // ostatecznie nikt nie mowil ze to ma byc alternatywa po neuronach
                // no to moze już na etapie przeprowadzania tej płaszczyzny.
                // wstawiamy zera tam gdzie trzeba, a jeśli chodzi o pozostałe rzeczy, mamy wolną rękę.
                // więc zapalamy mocno na minus te piksle które są w patternach przez które prowadzimy, a nie ma ich w wyróżnionej.
                // a mocno na plus zapalame te, które no odwrotnie.
                // zresztą idk czy motzno, może być +-1 i może wystarczy
                precision = ((double) same_side_and_label.size()) /
                        ((double) same_side_and_label.size() + (double) same_side_and_different_label.size());
                if(precision > best_precision){ // todo if cos innego
                    selected_by_best_hyperplane = same_side_and_label;
                    best_hyperplane = *(pattern.h);
                    best_precision = precision;
                    cout << "selecting neuron with tp " << same_side_and_label.size()
                    << ", fp " << same_side_and_different_label.size() << "\n";
                }
            }
        }
        Pattern nearest = get_nearest(selected_by_best_hyperplane, best_hyperplane);
        best_hyperplane.move_halfway_to_point(nearest.image.to_algebraic_vector());
        result.emplace_back(best_hyperplane,
                            best_hyperplane.on_positive_side(nearest.image.to_algebraic_vector()));
        dataset.remove_patterns(selected_by_best_hyperplane);
        cout << "removing " << selected_by_best_hyperplane.size() << "patterns\n" ;
        selected_by_best_hyperplane.clear();
    }
    return PerceptronNetwork{result};
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
    cout << "average neurons active for tp: " << reduce(howmany_recognize_tp.begin(), howmany_recognize_tp.end()) / (double) howmany_recognize_tp.size() << "\n.";
    cout << "average neurons active for fp: " << reduce(howmany_recognize_fp.begin(), howmany_recognize_fp.end()) / (double) howmany_recognize_fp.size() << "\n.";
}
