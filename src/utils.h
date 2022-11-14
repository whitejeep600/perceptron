#ifndef PERCEPTRON_UTILS_H
#define PERCEPTRON_UTILS_H
#include <vector>
#include <cstdint>
#include <memory>
#include <sys/resource.h>
#include <map>

#include "perceptron.h"

using namespace std;

double dot_product(const vector<double>& vec1, const vector<double>& vec2);

double euclidean_norm(const vector<double>& vec);

vector<double> mul_vector_by_scalar(vector<double> vec, double scalar);

bool column_has_only_zeroes(const Matrix& matrix, uint32_t col);

template<class T>
map<T, uint32_t> get_counts(const vector<T>& vec);

double average_of_ith_column(const Matrix& matrix, uint32_t column);

vector<Pattern> get_all_with_same_side_and_label(const Pattern& target, const Hyperplane& h, Dataset& dataset);

vector<Pattern> get_all_with_same_side_and_different_label(const Pattern& target,
                                                           const Hyperplane& h,
                                                           Dataset& dataset);
#endif //PERCEPTRON_UTILS_H
