#ifndef PERCEPTRON_PERCEPTRON_H
#define PERCEPTRON_PERCEPTRON_H

#include <cstdint>
#include <vector>
#include <set>
#include "hyperplane.h"

using namespace std;
using label = uint8_t;

const uint32_t IMAGE_HEIGHT = 28;
const uint32_t IMAGE_WIDTH = 28;
const uint32_t IMAGE_SIZE =  IMAGE_HEIGHT * IMAGE_WIDTH;

class Image{
public:
    uint8_t pixels[IMAGE_HEIGHT* IMAGE_WIDTH];
    explicit Image(const uint8_t* bytes);
    bool on_positive_side(const Hyperplane& h) const;
};

class Pattern{
public:
    const Image image;
    const label l;
    explicit Pattern(const Image& image, label l);
};

class Dataset{
public:
    set<Pattern> patterns;
    explicit Dataset(const vector<Image>& images, const vector<label>& labels);
    bool contains_label(label l) const;
    void remove_patterns(const set<Pattern>& to_remove);
};

class Perceptron{
public:
    Perceptron(Hyperplane h, bool on_positive_side);
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


vector<Perceptron> create_to_recognize(label l);

#endif //PERCEPTRON_PERCEPTRON_H
