#include <cassert>
#include <algorithm>
#include "perceptron.h"
#include "utils.h"

Dataset::Dataset(const vector<Image>& images, const vector<label>& labels)
:
patterns()
{
    assert(images.size() == labels.size());
    for(uint32_t i = 0; i < images.size(); ++i){
        patterns.emplace_back(images[0], labels[0]);
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

