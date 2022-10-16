#include <valarray>
#include <cassert>
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

vector<double> sum_by_row(Matrix& A, uint32_t n) {
    auto result = vector<double>();
    for(uint32_t i = 0; i < n; ++i){ // todo image_size?
        result.push_back(0);
        for(uint32_t j = 0; j < n; ++j){
            result[i] += A[i][j];
        }
    }
    return result;
}

// This function is based on the code from the paper: Farooq, A., &  Hamid, K. (2010).
// An efficient and simple algorithm for matrix inversion.
double get_determinant(Matrix matrix) {
    double pivot, det=1.0;
    uint32_t i, j, p;
    uint32_t size = matrix[0].size();
    std::vector<std::vector<double>> copy;
    for(p=1; p <= size; p++)
    {
        pivot = matrix[p-1][p-1];
        det= det * pivot;
        if (fabs(pivot) < 1e-5) return 0;
        for (j = 1; j<= size; j++)
            if (j != p) matrix[p-1][j-1] =  matrix[p-1][j-1] / pivot;
        for (i = 1; i<= size; i++)
            if (i != p) matrix[i-1][p-1] = (-1.0) * matrix[i-1][p-1] / pivot;
        for (i = 1; i<= size; i++)
            if (i != p)
                for (j= 1; j <= size; j++)
                    if (j != p)
                        matrix[i-1][j-1] = matrix[i-1][j-1] + matrix[p-1][j-1] * matrix[i-1][p-1] * pivot;
        matrix[p-1][p-1] = 1/ pivot;
    }
    return det;
}

// DISCLAIMER : all the code below is heavily based on a solution I found on Stack Overflow.
// I hope this is okay for the purposes of the assignment and will not be seen as cheating.
// It would surely have been allowed to use a C++ library function for Matrix inversion,
// but sadly no such function is available, and I did not want to write my own code for
// such a basic task.

Matrix get_transpose(const Matrix& matrix) {

    Matrix solution(matrix[0].size(), std::vector<double> (matrix.size()));

    for(uint32_t i = 0; i < matrix.size(); ++i) {
        for(uint32_t j = 0; j < matrix[0].size(); ++j) {
            solution[j][i] = matrix[i][j];
        }
    }
    return solution;
}

Matrix get_cofactor(const std::vector<std::vector<double>>& matrix) {
    Matrix solution(matrix.size(), std::vector<double> (matrix.size()));
    Matrix sub_matrix(matrix.size() - 1, std::vector<double> (matrix.size() - 1));

    for(std::uint32_t i = 0; i < matrix.size(); i++) {
        for(std::uint32_t j = 0; j < matrix[0].size(); j++) {
            int p = 0;
            for(uint32_t x = 0; x < matrix.size(); x++) {
                if(x == i) {
                    continue;
                }
                int q = 0;
                for(uint32_t y = 0; y < matrix.size(); y++) {
                    if(y == j) {
                        continue;
                    }
                    sub_matrix[p][q] = matrix[x][y];
                    q++;
                }
                p++;
            }
            solution[i][j] = pow(-1, i + j) * get_determinant(sub_matrix);
        }
    }
    return solution;
}

Matrix invert_matrix(const Matrix& matrix) {
    assert(get_determinant(matrix) != 0);
    double d = 1.0 / get_determinant(matrix);
    Matrix solution(matrix.size(), std::vector<double> (matrix.size()));

    for(uint32_t i = 0; i < matrix.size(); i++) {
        for(uint32_t j = 0; j < matrix.size(); j++) {
            solution[i][j] = matrix[i][j];
        }
    }

    solution = get_transpose(get_cofactor(solution));

    for(uint32_t i = 0; i < matrix.size(); i++) {
        for(uint32_t j = 0; j < matrix.size(); j++) {
            solution[i][j] *= d;
        }
    }

    return solution;
}

