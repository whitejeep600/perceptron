#include <valarray>
#include <map>
#include "utils.h"

#include "perceptron.h"

double dot_product(const vector<double>& vec1, const vector<double>& vec2) {
    double res = 0;
    for(uint32_t i = 0; i < vec1.size(); ++i){
        res += vec1[i] * vec2[i];
    }
    return res;
}

double euclidean_norm(const vector<double>& vec){
    double res = 0;
    for(auto x: vec){
        res += x*x;
    }
    return sqrt(res);
}

vector<double> mul_vector_by_scalar(vector<double> vec, double scalar){
    transform(vec.begin(),
              vec.end(),
              vec.begin(),
              [&scalar](double x){ return x * scalar; });
    return vec;
}

template<class T>
map<T, uint32_t> get_counts(const vector<T>& vec){
    auto values = set<T>();
    for(const auto howmany: vec){
        values.insert(howmany);
    }
    auto counts = std::map<T, uint32_t>();
    for(const auto value: values){
        counts.insert({value,
                       count(vec.begin(), vec.end(), value)});
    }
    return counts;
}

bool column_has_only_zeroes(const Matrix& matrix, uint32_t col){
    for(uint32_t i = 0; i < matrix.size(); ++i){
        if(matrix[i][col] != 0) return false;
    }
    return true;
}

double average_of_ith_column(const Matrix& matrix, uint32_t column){
    double sum = 0;
    for(uint32_t i = 0; i < matrix.size(); ++i){
        sum += matrix[i][column];
    }
    return sum / (double) matrix.size();
}

vector<Pattern> get_all_with_same_side(const Pattern& target, const Hyperplane& h, Dataset& dataset, bool same_label){
    vector<Pattern> result;
    for(Pattern& p: dataset.patterns){
        if(((p.l == target.l) == same_label) and h.on_same_side(target.image.to_algebraic_vector(),
                                              p.image.to_algebraic_vector())){
            result.push_back(p);
        }
    }
    return result;
}

vector<Pattern> get_all_with_same_side_and_label(const Pattern& target, const Hyperplane& h, Dataset& dataset){
    return get_all_with_same_side(target, h, dataset, true);
}

vector<Pattern> get_all_with_same_side_and_different_label(const Pattern& target,
                                                           const Hyperplane& h,
                                                           Dataset& dataset){
    return get_all_with_same_side(target, h, dataset, false);
}
