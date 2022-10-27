#ifndef PERCEPTRON_UTILS_H
#define PERCEPTRON_UTILS_H

#include <vector>
#include <cstdint>
#include <memory>
#include <sys/resource.h>
#include "perceptron.h"

using namespace std;

template <class T>
bool vector_contains(const std::vector<T>& vec, const T item){
    return find(vec.begin(), vec.end(), item) != vec.end();
}

double dot_product(const vector<double>& vec1, const vector<double>& vec2);

double euclidean_norm(const vector<double>& vec);

vector<double> mul_vector_by_scalar(vector<double> vec, double scalar);

vector<double> sum_by_row(Matrix& A, uint32_t n) ;

Matrix invert_matrix(const Matrix& matrix);

bool column_has_only_zeroes(const Matrix& matrix, uint32_t col);

vector<uint32_t> find_columns_of_zeroes(const Matrix& matrix);

#endif //PERCEPTRON_UTILS_H
