#ifndef PERCEPTRON_HYPERPLANE_H
#define PERCEPTRON_HYPERPLANE_H

#include <cstdint>
#include <vector>

using namespace std;

class Hyperplane{
public:
    vector<double> coefficients_vector;
    double constant_term;
    // the equation describing the hyperplane is coefficients_vector^{T} * x = constant_term
    //void move_halfway_to_point(const uint8_t point[784]);
    Hyperplane(const vector<double>& coefficients_vector, double constant_term);
    Hyperplane();
    bool on_positive_side(const vector<double>& vec) const;
    bool on_same_side(const vector<double>& vec1, const vector<double>& vec2) const;
    double distance_to_point(const vector<double>& vec) const;
};


#endif
