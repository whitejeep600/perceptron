#include <valarray>
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

vector<double> sum_by_row(std::unique_ptr<Eigen::Matrix<double, IMAGE_SIZE, IMAGE_SIZE>> A) {
    auto result = vector<double>();
    for(uint32_t i = 0; i < IMAGE_SIZE; ++i){
        result.push_back(0);
        for(uint32_t j = 0; j < IMAGE_SIZE; ++j){
            result[i] += (*A)(i, j);
        }
    }
    return result;
}

void increase_stack_size(){
    const rlim_t stack_size = 67108864; // 64 MiB
    struct rlimit rlim;
    rlim.rlim_cur = stack_size;
    setrlimit(RLIMIT_STACK, &rlim);
}