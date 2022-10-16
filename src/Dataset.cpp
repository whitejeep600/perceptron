#include <cassert>
#include <algorithm>
#include <fstream>
#include <iostream>
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
    for(auto target: to_remove){
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

// Let the plane equation in n dimensions be sum(c_n * x_n = 1) (this is assuming that
// zero does not belong to the plane but that is extremely unlikely to happen). Then let c
// be the vector of c_i. We are given a set of vectors belonging to that plane. If we
// assemble these vectors into matrix A so that each vector becomes a row, c satisfies
// Ax = 1 (vector of ones). Then x = A^{-1} * 1.
Hyperplane lead_through(const vector<Pattern>& patterns){
    //matrix A = {{0}}; todo zamienic z powrotem po przetestowaniu
    auto A = std::vector<std::vector<double>>(IMAGE_SIZE);
    uint32_t row = 0;
    assert(patterns.size() == IMAGE_SIZE);
    for(const auto& p: patterns){
        A[row] = std::vector<double>(IMAGE_SIZE);
        for(uint32_t column = 0; column < IMAGE_SIZE; ++column){
            A[row][column] = p.image.pixels[column];
        }
        ++row;
    }
    invert_matrix(A);
    vector<double> coefficients_vector = sum_by_row(A, IMAGE_SIZE);
    return {coefficients_vector, 1};
}

// actually we could dump to file
// raz dumpujemy i nie zachowujemy kodu do dumpowania; potem preprocess uruchamiane jest z argumentem
// bool from_preprocessed który informuje czy wczytać z dumpa czy stworzyć na nowo

void Dataset::preprocess(label l, bool dump) {
    // This only needs to be done once for each pattern while training to recognize a single label.
    // Also it begs for parallelizing, computation for each pattern is independent
    uint32_t i = 0;
    std::ofstream dump_file;
    if(dump){
        dump_file = std::ofstream("train_hyperplanes_dump.txt");
    }
    for(Pattern& p: patterns){
        if(p.l == l) {
            vector<Pattern> nearest_different_label = get_nearest_with_different_label(p, 784, *this);
            p.h = make_shared<Hyperplane>(lead_through(nearest_different_label));
            if(dump){
                dump_file << "sample number " << i << " constant " << (*(p.h)).constant_term << " vector: ";
                for (const double c : (*(p.h)).coefficients_vector) dump_file << c << " ";
                dump_file << "\n";
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
    cout << "read all";
}


