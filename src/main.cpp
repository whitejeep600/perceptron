#include <algorithm>
#include <iostream>
#include <getopt.h>
#include "perceptron.h"

int main(int argc, char** argv) {
    int32_t option_char;
    label l = 0;
    bool from_preprocessed = false;
    while ((option_char = getopt(argc, argv, "p")) != -1){
        if(option_char == 'p') {
            from_preprocessed = true;
        }
    }
    auto network = create_to_recognize(l, read_train_dataset(), from_preprocessed);
    cout << "Testing on the test dataset.\n";
    network.test_on_dataset(read_test_dataset(), l, true);
}
