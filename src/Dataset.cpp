#include <cassert>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <functional>
#include <random>
#include "perceptron.h"
#include "utils.h"

Dataset::Dataset(const vector<Image>& images, const vector<label>& labels)
:
patterns()
{
    assert(images.size() == labels.size());
    for(uint32_t i = 0; i < images.size(); ++i){
        patterns.emplace_back(images[i], labels[i]);
    }
}

bool Dataset::contains_label(label l) const {
    for(Pattern p: this->patterns){
        if(p.l == l){
            return true;
        }
    }
    return false;
}

Dataset::Dataset(const vector<Pattern> &patterns)
:
patterns(patterns)
{}

// I know this is ugly but using a hashset or something in cpp is pain :)
void Dataset::remove_patterns(const vector<Pattern>& to_remove){
    uint32_t temp_size = 0;
    for(auto& target: to_remove){
        auto iter = find(patterns.begin(), patterns.end(), target);
        if(iter != patterns.end()){
            temp_size = patterns.size();
            patterns.erase(iter);
            assert(patterns.size() < temp_size);
        }
    }
}

vector<Pattern> get_nearest_with_different_label(const Pattern& pattern, uint32_t how_many, Dataset dataset){
    struct key
    {
        Pattern target_pattern;
        explicit key(Pattern p): target_pattern(p){}
        bool operator() (const Pattern& pattern1, const Pattern& pattern2) const
        {
            return (pattern1.image.euclidean_distance_squared(target_pattern.image) <
                    pattern2.image.euclidean_distance_squared(target_pattern.image));
        }
    };
    auto sort_key = key(pattern);
    std::sort(dataset.patterns.begin(), dataset.patterns.end(), sort_key);
    auto result = vector<Pattern>();
    uint32_t found = 0;
    uint32_t i = 0;
    while(found < how_many){
        if(dataset.patterns[i].l != pattern.l){
            result.push_back(dataset.patterns[i]);
            ++found;
        }
        ++i;
    }
    return result;
}

// dobra chyba trzeba po prostu inaczej rozwiązać ren układ równań, tylko pamiętać o zaburzeniu macierzy.
// nie wiem może jakieś przybliżenie iteracyjne
// albo przepisać na pytonga i svd
// albo po prostu dobrać jakieś arbitralne wartości tam gdzie jest niejednoznaczność
// tworzenie hiperpłaszczyzny z svm?
// a może np iteracyjnie? dopóki maciora jest osobliwa to dopisujemy do coefficients jedynkę i idziemy dalej
// albo zero
// nie no to się dobrze nie skończy bo ostatnia kolumna też raczej zawiera same zera
// może jest jakiś algorytm żeby znaleźć JAKIEŚ rozwiązanie niejednoznacznego układu równań. bo on przecież
// sprzeczny raczej nie jest
// może znajdowanie kolumn z samymi zerami i jeśli kolumna p ma same zera, to można wywalić kolumnę
// i wiersz p i wpisać jedynkę do coefficients vector? i znaleźć rozwiązanie dla reszty


bool column_has_only_zeroes(const Matrix& matrix, uint32_t col){
    for(uint32_t i = 0; i < matrix.size(); ++i){
        if(matrix[i][col] != 0) return false;
    }
    return true;
}

vector<uint32_t> find_columns_of_zeroes(const Matrix& matrix){
    vector<uint32_t> res;
    for(uint32_t i = 0; i < matrix.size(); ++i){
        if(column_has_only_zeroes(matrix, i)) res.push_back(i);
    }
    return res;
}

// todo mamy za zadanie znaleźć takie x, Mx = 1
// no ale hola, nie
// jeżeli któryś wiersz jest cały zerami to i tak się nie da tego zrobić,
// więc jest jakoś jeszcze dziwniej
// może na przypadki? jeśli jest kolumna z samymi zerami (albo macierz osobliwa?) to musi przechodzic
// przez środek układu współrzędnych i wtedy constant_term jest 0? a jeśli nie to jest 1
// no to zaraz ... jeżeli to ma przechodzić przez środek układu współrzędnych to jest równanie
// Ax = 0 i można podstawić za iksa zero i już
// może poprosić o konsultację z jiaoshou i już

class Rand_double
{
public:
    Rand_double(double low, double high)
            :r(std::bind(std::uniform_real_distribution<>(low,high),std::default_random_engine())){}

    double operator()(){ return r(); }

private:
    std::function<double()> r;
};

Hyperplane lead_through(const vector<Pattern>& patterns){
    auto matrix = Matrix (IMAGE_SIZE);
    uint32_t row = 0;
    assert(patterns.size() == IMAGE_SIZE);
    for(const auto& p: patterns){
        matrix[row] = std::vector<double>(IMAGE_SIZE);
        for(uint32_t column = 0; column < IMAGE_SIZE; ++column){
            matrix[row][column] = p.image.pixels[column];
        }
        ++row;
    }
    vector<double> res;
    bool at_least_one_zero = false;
    Rand_double rd{-1,1};
    for(uint32_t i = 0; i < matrix.size(); ++i){
        if(column_has_only_zeroes(matrix, i)){
            res.push_back(rd());
        }
        else{
            res.push_back(0.0);
            at_least_one_zero = true;
        }
    }
    assert(at_least_one_zero);
    return {res, 0.0};

}

void Dataset::preprocess(label l, bool dump) {
    // This only needs to be done once for each pattern while training to recognize a single label.
    // Also it begs for parallelizing, computation for each pattern is independent
    uint32_t i = 0;
    std::ofstream dump_file;
    if(dump){
        dump_file = std::ofstream("../data/train_hyperplanes_dump.txt");
    }
    for(Pattern& p: patterns){
        if(p.l == l) {
            vector<Pattern> nearest_different_label = get_nearest_with_different_label(p, 784, *this);
            p.h = make_shared<Hyperplane>(lead_through(nearest_different_label));
            if(dump){
                dump_file << "sample number " << i << " constant " << (*(p.h)).constant_term << " vector: ";
                for (const double c : (*(p.h)).coefficients_vector) dump_file << c << " ";
                dump_file << "\n";
                cout << i << "\n";
            }
        }
        ++i;
    }
    if(dump){
        dump_file.close();
    }
}

void Dataset::preprocess_from_dump(label l) {
    uint32_t i = 0;
    auto dump_file = std::ifstream("../data/train_hyperplanes_dump.txt");
    string input_token;
    double constant_term;
    vector<double> coefficients;
    for(Pattern& p: patterns){
        coefficients.clear();
        if(p.l == l) {
            dump_file >> input_token;
            assert(input_token == "sample");
            dump_file >> input_token;
            assert(input_token == "number");
            dump_file >> input_token;
            assert(i == stoul(input_token));
            dump_file >> input_token;
            assert(input_token == "constant");
            dump_file >> input_token;
            constant_term = std::stod(input_token) ;
            dump_file >> input_token;
            assert(input_token == "vector:");
            for(uint32_t j = 0; j < IMAGE_SIZE; ++j){
                dump_file >> input_token;
                coefficients.push_back(std::stod(input_token));
            }
            p.h = make_shared<Hyperplane>(Hyperplane(coefficients, constant_term));
        }
        ++i;
    }
    // assert(!(dump_file >> constant_term)); // making sure everything was read from the file todo
    dump_file.close();
}


