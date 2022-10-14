#include <cassert>
#include <algorithm>
#include <iostream>
#include "perceptron.h"
#include "test.h"
#include "utils.h"

int main() {
    increase_stack_size();
    auto train_dataset = read_train_dataset();
    label l = 0;
    auto perceptrons = create_to_recognize(l, train_dataset);
    return 0;
}
