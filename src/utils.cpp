#include "utils.h"


double dot_product(const vector<double>& vec1, const vector<double>& vec2) {
    double res = 0;
    for(uint32_t i = 0; i < vec1.size(); ++i){
        res += vec1[i] * vec2[i];
    }
    return res;
}
