#include <iostream>
#include "perceptron.h"

int main() {
    read_train_labels();
    read_test_labels();
    read_train_images();
    read_test_images();
}


// jest klasa sample ktora zawiera obrazek i label i może tam jakieś id
// jest klasa set ktora zawiera wektor sampli albo jest wektorem sampli