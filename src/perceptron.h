#ifndef PERCEPTRON_PERCEPTRON_H
#define PERCEPTRON_PERCEPTRON_H

#include <cstdint>
#include <vector>
#include <set>
#include <memory>
#include "hyperplane.h"

using namespace std;
using label = uint8_t;


const uint32_t IMAGE_HEIGHT = 28; // todo
const uint32_t IMAGE_WIDTH = 28;
const uint32_t IMAGE_SIZE =  IMAGE_HEIGHT * IMAGE_WIDTH;

using matrix = double[IMAGE_SIZE][IMAGE_SIZE];

class Image{
public:
    uint8_t pixels[IMAGE_HEIGHT* IMAGE_WIDTH];
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
    explicit Dataset(const vector<Pattern>& patterns);
    bool contains_label(label l) const;
    void remove_patterns(const vector<Pattern>& to_remove);
    void preprocess(label l);
};

class Perceptron{
private:
    Hyperplane h;
    bool recognizes_positive_side;
public:
    Perceptron(Hyperplane  h, bool on_positive_side);
};

// mapuje labele na wektory perceptronów, które ją mają rozpoznawać
class PerceptronNetwork{
public:
    // void test_on_dataset(Dataset test_dataset) const;
    // label infer_for_image(const Image& image) const;
};

Dataset read_train_dataset();
Dataset read_test_dataset();

PerceptronNetwork create_for_dataset(const Dataset& test_dataset);

vector<Perceptron> create_to_recognize(label l, Dataset dataset);

// todo niepotrzebne rzeczy z naglowkow do wywalenia

vector<Pattern> get_all_with_same_side_and_label(const Pattern& p, const Hyperplane& h, Dataset& dataset);
Pattern get_nearest(vector<Pattern>& patterns, const Hyperplane& h);

vector<Pattern> get_nearest_with_different_label(const Pattern& pattern, uint32_t how_many, Dataset& dataset);

Hyperplane lead_through(const vector<Pattern>& patterns);
#endif //PERCEPTRON_PERCEPTRON_H
