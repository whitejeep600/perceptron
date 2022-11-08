#include <valarray>
#include <cassert>
#include "utils.h"


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


// DISCLAIMER : all the code below is heavily based on solutions I found on internet forums.
// I hope this is okay for the purposes of the assignment and will not be seen as cheating.
// It would surely have been allowed to use a C++ library function for Matrix inversion,
// but sadly no such function is available, and I did not want to write my own code for
// such a basic task.
double get_determinant(Matrix matrix) {
    uint32_t size = matrix.size();
    double num1, num2, det = 1;
    uint32_t index;
    double total = 1;
    double temp[size + 1];
    for (uint32_t i = 0; i < size; i++)
    {
        index = i;
         while (index < size && matrix[i][index] == 0){
             index++;
         }
         if (index == size){
             continue;
             }
         if (index != i){
             for (uint32_t j = 0; j < size; j++){
                 swap(matrix[index][j], matrix[i][j]);
             }
             det *= pow(-1, index - i);
         }
         for (uint32_t j = 0; j < size; j++){
             temp[j] = matrix[i][j];
         }
         for (uint32_t j = i + 1; j < size; j++){
             num1 = temp[i];
             num2 = matrix[j][i];
             for (uint32_t k = 0; k < size; k++){
                 matrix[j][k] = (num1 * matrix[j][k]) - (num2 * temp[k]);
             }
             total *=  num1;
         }
    }
    for (uint32_t i = 0; i < size; i++){
        det *= matrix[i][i];
    }
    if(total == 0) return 0;
    return (det / total);
}

bool invertible(const Matrix& matrix){
    return get_determinant(matrix) != 0;
}

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
    double det = get_determinant(matrix);
    assert(det != 0);
    double d = 1.0 / det;
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

vector<uint32_t> find_columns_of_zeroes(const Matrix& matrix){
    vector<uint32_t> res;
    for(uint32_t i = 0; i < matrix.size(); ++i){
        if(column_has_only_zeroes(matrix, i)) res.push_back(i);
    }
    return res;
}

bool column_has_only_zeroes(const Matrix& matrix, uint32_t col){
    for(uint32_t i = 0; i < matrix.size(); ++i){
        if(matrix[i][col] != 0) return false;
    }
    return true;
}

