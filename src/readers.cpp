#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <cassert>
#include "perceptron.h"

using namespace std;

vector<label> read_labels(const char* filename, uint32_t expected_num_labels){
    ifstream f(filename, ios::binary | ios::ate);
    f.seekg(0, std::ifstream::end);
    uint32_t length = f.tellg();
    f.seekg(0, std::ifstream::beg);

    char *buffer = new char[length];
    f.read(buffer, length);
    f.close();

    uint32_t magic_number = ntohl(*(uint32_t*) buffer);
    assert(magic_number == 2049);

    uint32_t num_labels = ntohl(*(uint32_t*) (buffer + 4));
    assert(num_labels == expected_num_labels);

    vector<label> labels;

    label new_label;
    for(uint32_t i = 0; i < num_labels; ++i){
        new_label = *(uint8_t*) (buffer + 8 + i);
        assert(0 <= new_label);
        assert(new_label <= 9);
        labels.push_back(new_label);
    }

    delete[] buffer;
    return labels;
}

std::vector<label> read_train_labels(){
    return read_labels("../data/train_labels", 60000);
}
std::vector<label> read_test_labels(){
    return read_labels("../data/test_labels", 10000);
}

vector<Image> read_images(const char* filename, uint32_t expected_num_images){
    ifstream f(filename, ios::binary | ios::ate);
    f.seekg(0, std::ifstream::end);
    uint32_t length = f.tellg();
    assert(length == 16 + expected_num_images * IMAGE_SIZE);
    f.seekg(0, std::ifstream::beg);

    char *buffer = new char[length];
    f.read(buffer, length);
    f.close();

    uint32_t magic_number = ntohl(*(uint32_t*) buffer);
    assert(magic_number == 2051);

    uint32_t num_images = ntohl(*(uint32_t*) (buffer + 4));
    assert(num_images == expected_num_images);

    uint32_t image_width = ntohl(*(uint32_t*) (buffer + 8));
    assert(image_width == IMAGE_WIDTH);

    uint32_t image_height = ntohl(*(uint32_t*) (buffer + 12));
    assert(image_height == IMAGE_HEIGHT);

    vector<Image> images;
    for(uint32_t i = 0; i < num_images; ++i){
        images.emplace_back((uint8_t*)  buffer + 16 + i * IMAGE_SIZE);
    }
    return images;
}

std::vector<Image> read_train_images(){
    return read_images("../data/train_images", 60000);
}
std::vector<Image> read_test_images(){
    return read_images("../data/test_images", 10000);
}

Dataset read_train_dataset(){
    return Dataset(read_train_images(), read_train_labels());
}
Dataset read_test_dataset(){
    return Dataset(read_test_images(), read_test_labels());
}