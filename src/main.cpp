#include <algorithm>
#include "perceptron.h"


// do raportu: rozkmina z przeprowadzaniem hiperpłaszczyzny, z przesunięciem o wektor (algebra),
// eigen i problem
// z pamięcią, problem z czasem wykonania i dumpowanie do pliku - fajnie że jest w ogóle możliwe bo
// wykonywane są pewne obliczenia raz dla każdego patternu i mają łatwy do zdumpowania wynik.
// paper XD
// ran but all false positives, realized Matrix inversions don't work because the algorithm was
// bad because no pivoting
// eh realized the hyperplane stuff is tricky and just leading a hyperplane won't give much
// started assigning zeroes to non-zero columns and random values (-1, 1) to zero ones
// F1 score: 0.178946
// precision: 0.0982653, recall: 1. tbh given the selection method, that was to be expected
// maximizing distance to hyperplane heuristic
// todo Each pattern has  many zeros in its grey level image. It is suggested, calculate the mean of
//  the grey pixels for each image and normalize each grey pixel in between (-1,1) to easy the
//  algebra problems in computations of the hyperplane.
//   ^ tak, to ma sens. nie wiem czy nie lepiej between (-4, 4) np. poniewaz numeryka
// apply some heuristic
// to choosing the arbitrary coefficients I mentioned in the previous
// e-mail, or slightly perturb each of the training patterns so that they
// become numerically independent
// When these 784 patterns are degenerative with less dimensions, < 784, one can still work out the one with maximum distance.
// Later in the class, I will show you a simple approach included in Chapter 5.
// choosing the constant adjusting the constant term
// choosing the behavior of the final neuron
// todo constant i private co mozna
int main() {
    auto train_dataset = read_train_dataset();
    label l = 0;
    auto network = create_to_recognize(l, train_dataset, true);
    network.test_on_dataset(read_test_dataset(), l);
    return 0;
}
