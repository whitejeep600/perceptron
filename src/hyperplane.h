#ifndef PERCEPTRON_HYPERPLANE_H
#define PERCEPTRON_HYPERPLANE_H

#include <cstdint>
#include <vector>

using namespace std;

class Hyperplane{
public:
    vector<double> coefficients_vector;
    double constant_term;
    void move_halfway_to_point(const uint8_t point[784]);
    Hyperplane(const vector<double>& coefficients_vector, double constant_term);
};


#endif
