#include <cassert>
#include <algorithm>
#include <iostream>
#include "perceptron.h"
#include "test.h"
#include "utils.h"

// todo przepisać na nieużywanie eigen, pamiętając o alokacjach na stercie i delete na wszelki wypadek;
// może jednak jeszcze jeden test xd
// zdumpować do pliku; i napisać wczytywanie z pliku
// do raportu: rozkmina z przeprowadzaniem hiperpłaszczyzny, z przesunięciem o wektor, eigen i problem
// z pamięcią, problem z czasem wykonania i dumpowanie do pliku - fajnie że jest w ogóle możliwe bo
// wykonywane są pewne obliczenia raz dla każdego patternu i mają łatwy do zdumpowania wynik.
// pakistański paper XDDDDDDD
int main() {
    const rlim_t stack_size = 67108864; // 64 MiB
    struct rlimit rlim;
    rlim.rlim_cur = stack_size;
    setrlimit(RLIMIT_STACK, &rlim);
    auto train_dataset = read_train_dataset();
    label l = 0;
    auto perceptrons = create_to_recognize(l, train_dataset);
    return 0;
}
