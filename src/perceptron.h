#ifndef PERCEPTRON_PERCEPTRON_H
#define PERCEPTRON_PERCEPTRON_H

#include <cstdint>
#include <utility>
#include <vector>
#include <set>
#include <memory>
#include "hyperplane.h"

using namespace std;
using label = uint8_t;
using Matrix = std::vector<std::vector<double>>;

const uint32_t IMAGE_HEIGHT = 28;
const uint32_t IMAGE_WIDTH = 28;
const uint32_t IMAGE_SIZE =  IMAGE_HEIGHT * IMAGE_WIDTH;

class Image{
private:
    vector<double> create_algebraic_vector() const;
public:
    double pixels[IMAGE_HEIGHT* IMAGE_WIDTH];
    explicit Image(const uint8_t* bytes);
    double euclidean_distance_squared(const Image& image2) const;
    bool operator==(const Image& that) const;
    vector<double> to_algebraic_vector() const;
};

class Pattern{
public:
    Image image;
    label l;
    std::shared_ptr<Hyperplane> h;
    explicit Pattern(const Image& image, label l);
    bool operator==(const Pattern& that) const;
};

class Dataset{
public:
    vector<Pattern> patterns;
    explicit Dataset(const vector<Image>& images, const vector<label>& labels);
    bool contains_label(label l) const;
    void remove_patterns(const vector<Pattern>& to_remove);
    void preprocess(label l, bool dump_to_file);
    void preprocess_from_dump(label l);
};

Dataset read_train_dataset();
Dataset read_test_dataset();

class Perceptron{
public:
    bool recognizes_positive_side;
    Hyperplane h;
    Perceptron(const Hyperplane& h, bool on_positive_side);
    bool recognizes(const Pattern& p) const;
};

class PerceptronNetwork{
private:
    uint32_t treshold;
    bool recognizes(const Pattern& p) const;
    uint32_t howmany_recognize(const Pattern& p) const;

public:
    vector<Perceptron> perceptrons;
    explicit PerceptronNetwork(const vector<Perceptron>& perceptrons, uint32_t treshhold)
    :
    treshold(treshhold), perceptrons(perceptrons)  {}
    double test_on_dataset(const Dataset& dataset, label l, bool print_details) const;
    void dump_to_file() const;

};

PerceptronNetwork create_to_recognize(label l, Dataset&& dataset, bool from_preprocessed);
PerceptronNetwork read_from_file();

Hyperplane lead_through_or_above(const vector<Pattern>& patterns, const Pattern& target);

#endif //PERCEPTRON_PERCEPTRON_H
