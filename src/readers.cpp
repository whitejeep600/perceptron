#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <cassert>
#include "perceptron.h"

using namespace std;

vector<label> read_labels(const char* filename){
    ifstream f(filename, ios::binary | ios::ate);
    f.seekg(0, std::ifstream::end);
    uint32_t length = f.tellg();
    f.seekg(0, std::ifstream::beg);

    char *buffer = new char[length];
    f.read(buffer, length);
    f.close();

    uint32_t magic_number = ntohl(*(uint32_t*) buffer);
    assert(magic_number == 2049);

    uint32_t num_labels = ntohl(*(uint32_t*) (buffer+4));
    assert(num_labels == 60000);

    vector<label> labels;

    label new_label;
    for(uint32_t i = 0; i < num_labels; ++i){
        new_label = *(uint8_t*) (buffer+8+i);
        assert(0 <= new_label);
        assert(new_label <= 9);
        labels.push_back(new_label);
    }

    delete[] buffer;
    return labels;
}