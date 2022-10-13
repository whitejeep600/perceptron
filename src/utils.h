#ifndef PERCEPTRON_UTILS_H
#define PERCEPTRON_UTILS_H

#include <vector>
#include <cstdint>

using namespace std;

template <class T>
bool vector_contains(const std::vector<T>& vec, const T item){
    return find(vec.begin(), vec.end(), item) != vec.end();
}

double dot_product(const vector<double>& vec1, const vector<double>& vec2);

#endif //PERCEPTRON_UTILS_H
