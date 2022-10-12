#include <cassert>
#include "perceptron.h"

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

//void Dataset::remove_patterns(const set<Pattern>& to_remove){
//    // todo
//}

