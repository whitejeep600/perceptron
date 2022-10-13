#include <cassert>
#include <algorithm>
#include "perceptron.h"

void test(){
    uint8_t bytes[3] = {10, 10, 10};
    auto image1 = Image(bytes);

    bytes[0] = 20;
    bytes[1] = 20;
    bytes[2] = 20;
    auto image2 = Image(bytes);

    bytes[0] = 10;
    bytes[1] = 10;
    bytes[2] = 0;
    auto image3 = Image(bytes);

    bytes[0] = 10;
    bytes[1] = 15;
    bytes[2] = 10;
    auto image4 = Image(bytes);

    bytes[0] = 10;
    bytes[1] = 10;
    bytes[2] = 10;
    auto image5 = Image(bytes);

    bytes[0] = 15;
    bytes[1] = 10;
    bytes[2] = 10;
    auto image6 = Image(bytes);

    auto pattern1 = Pattern(image1, 1);
    auto pattern2 = Pattern(image2, 4);
    auto pattern3 = Pattern(image3, 2);
    auto pattern4 = Pattern(image4, 1);
    auto pattern5 = Pattern(image5, 1);
    auto pattern6 = Pattern(image6, 6);
    // for 1 nearest, should
    vector<Pattern> patterns{pattern1, pattern2, pattern3, pattern4, pattern5, pattern6};
    auto dataset = Dataset(patterns);
    auto vec1 = get_nearest_with_different_label(pattern1, 1, dataset);
    assert(vec1.size() == 1);
    assert(find(vec1.begin(), vec1.end(), pattern6) != vec1.end());
    auto vec2 = get_nearest_with_different_label(pattern1, 2, dataset);
    assert(vec2.size() == 2);
    assert(find(vec2.begin(), vec2.end(), pattern6) != vec2.end());
    assert(find(vec2.begin(), vec2.end(), pattern3) != vec2.end());
    auto vec3 = get_nearest_with_different_label(pattern1, 3, dataset);
    assert(vec3.size() == 3);
    assert(find(vec3.begin(), vec3.end(), pattern6) != vec3.end());
    assert(find(vec3.begin(), vec3.end(), pattern3) != vec3.end());
    assert(find(vec3.begin(), vec3.end(), pattern2) != vec3.end());
}
