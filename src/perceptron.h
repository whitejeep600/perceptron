#ifndef PERCEPTRON_PERCEPTRON_H
#define PERCEPTRON_PERCEPTRON_H

#include <cstdint>
#include <vector>

using namespace std;
using label = uint8_t;

const uint32_t IMAGE_HEIGHT = 28;
const uint32_t IMAGE_WIDTH = 28;
const uint32_t IMAGE_SIZE =  IMAGE_HEIGHT * IMAGE_WIDTH;

class Image{
private:
    uint8_t pixels[IMAGE_HEIGHT][IMAGE_WIDTH];
public:
    explicit Image(const uint8_t* bytes);
};
class Sample;
class Dataset;
class PerceptronNetwork{
public:
    // void test_on_dataset(Dataset test_dataset);
};

std::vector<label> read_train_labels();
std::vector<label> read_test_labels();

std::vector<Image> read_train_images();
std::vector<Image> read_test_images();

Dataset read_train_dataset();
Dataset read_test_dataset();

PerceptronNetwork train_on_dataset(Dataset test_dataset);

#endif //PERCEPTRON_PERCEPTRON_H
