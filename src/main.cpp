#include <algorithm>
#include <iostream>
#include <getopt.h>
#include "perceptron.h"

int main(int argc, char** argv) {
    int32_t option_char;
    label l = 0;
    bool just_test = false;
    bool from_preprocessed = false;
    while ((option_char = getopt(argc, argv, "pt")) != -1){
        switch (option_char) {
            case('p'):
                from_preprocessed = true;
                break;
            case('t'):
                just_test = true;
                break;
            default:
                break;
        }
    }
    if(not just_test){
        auto network = create_to_recognize(l, read_train_dataset(), from_preprocessed);
        network.dump_to_file();
        cout << "Testing on the test dataset.\n";
        network.test_on_dataset(read_test_dataset(), l, true);
        for(auto& p: network.perceptrons){
            cout << "constant";
            cout << p.h.constant_term;
            cout << "positive" << p.recognizes_positive_side << " ";
            for(auto r: p.h.coefficients_vector){
                cout << r << " ";;
            }
        }
    }
    else{
        auto network = read_from_file();
        cout << "Testing on the test dataset.\n";
        network.test_on_dataset(read_test_dataset(), l, true);
    }
    return 0;
}
