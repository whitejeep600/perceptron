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

vector<double> sum_by_row(std::vector<std::vector<double>>& A, uint32_t n) {
    auto result = vector<double>();
    for(uint32_t i = 0; i < n; ++i){ // todo image_size?
        result.push_back(0);
        for(uint32_t j = 0; j < n; ++j){
            result[i] += A[i][j];
        }
    }
    return result;
}

double invert_matrix(std::vector<std::vector<double>>& vect) {
    double pivot, det=1.0;
    uint32_t i, j, p;
    uint32_t size = vect[0].size();
    std::vector<std::vector<double>> copy;
    for(p=1; p <= size; p++)
    {
        pivot = vect[p-1][p-1];
        det= det * pivot;
        if (fabs(pivot) < 1e-5) return 0;
        for (j = 1; j<= size; j++)
            if (j != p) vect[p-1][j-1] =  vect[p-1][j-1] / pivot;
        for (i = 1; i<= size; i++)
            if (i != p) vect[i-1][p-1] = (-1.0) * vect[i-1][p-1] / pivot;
        for (i = 1; i<= size; i++)
            if (i != p)
                for (j= 1; j <= size; j++)
                    if (j != p)
                        vect[i-1][j-1] = vect[i-1][j-1] + vect[p-1][j-1] * vect[i-1][p-1] * pivot;
        vect[p-1][p-1] = 1/ pivot;
    }
    return det;

}

