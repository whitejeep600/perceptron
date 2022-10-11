#ifndef PERCEPTRON_PERCEPTRON_H
#define PERCEPTRON_PERCEPTRON_H

#include <cstdint>
#include <vector>

using namespace std;
using label = uint8_t;

class Image;
class Sample;
class Dataset;
class PerceptronNetwork{
public:
    // void test_on_dataset(Dataset test_dataset);
};

std::vector<label> read_labels(const char* filename);

Dataset read_train_dataset();
Dataset read_test_dataset();

PerceptronNetwork train_on_dataset(Dataset test_dataset);

#endif //PERCEPTRON_PERCEPTRON_H
