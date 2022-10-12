#ifndef PERCEPTRON_HYPERPLANE_H
#define PERCEPTRON_HYPERPLANE_H

#include <cstdint>

class Hyperplane{
public:
    void move_halfway_to_point(const uint8_t point[784]);
};


#endif
