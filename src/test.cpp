#include <cassert>
#include <algorithm>
#include "perceptron.h"
#include "utils.h"

// for this to work, change IMAGE_WIDTH to 3 and HEIGHT to 1 and comment out any
// code in other functions that results in static check fails :)
void test_get_nearest_with_different_label(){
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
    vector<Pattern> patterns{pattern1, pattern2, pattern3, pattern4, pattern5, pattern6};
    auto dataset = Dataset(patterns);
    auto vec1 = get_nearest_with_different_label(pattern1, 1, dataset);
    assert(vec1.size() == 1);
    assert(vector_contains<Pattern>(vec1, pattern6));
    auto vec2 = get_nearest_with_different_label(pattern1, 2, dataset);
    assert(vec2.size() == 2);
    assert(vector_contains<Pattern>(vec2, pattern6));
    assert(vector_contains<Pattern>(vec2, pattern3));
    auto vec3 = get_nearest_with_different_label(pattern1, 3, dataset);
    assert(vec3.size() == 3);
    assert(vector_contains<Pattern>(vec3, pattern6));
    assert(vector_contains<Pattern>(vec3, pattern2));
    assert(vector_contains<Pattern>(vec3, pattern2));
}

// for this to work, both width and height = 3
void test_get_with_same_side_and_label(){
    uint8_t bytes[9] = {1, 0, 0, 0, 0, 0, 0, 0, 0};
    auto image1 = Image(bytes);

    bytes[0] = 0;
    bytes[1] = 1;
    bytes[2] = 0;
    auto image2 = Image(bytes);

    bytes[0] = 0;
    bytes[1] = 0;
    bytes[2] = 1;
    auto image3 = Image(bytes);

    auto pattern1 = Pattern(image1, 1);
    auto pattern2 = Pattern(image2, 1);
    auto pattern3 = Pattern(image3, 1);
    vector<Pattern> patterns{pattern1, pattern2, pattern3};
    auto h = lead_through(patterns);

    bytes[0] = 2;
    bytes[1] = 2;
    bytes[2] = 2;
    auto target_pattern = Pattern(Image(bytes), 2);

    bytes[0] = 2;
    bytes[1] = 2;
    bytes[2] = 3;
    auto close_same_label = Pattern(Image(bytes), 2);

    bytes[0] = 2;
    bytes[1] = 3;
    bytes[2] = 3;
    auto close_different_label = Pattern(Image(bytes), 5);

    bytes[0] = 100;
    bytes[1] = 200;
    bytes[2] = 250;
    auto far_same_label = Pattern(Image(bytes), 2);

    bytes[0] = 0;
    bytes[1] = 0;
    bytes[2] = 0;
    auto other_side_same_label = Pattern(Image(bytes), 2);

    bytes[0] = 0;
    bytes[1] = 0;
    bytes[2] = 0;
    auto other_side_different_label = Pattern(Image(bytes), 4);

    assert(h.on_positive_side(target_pattern.image.to_algebraic_vector()));
    assert(h.on_positive_side(close_same_label.image.to_algebraic_vector()));
    assert(h.on_positive_side(close_different_label.image.to_algebraic_vector()));
    assert(h.on_positive_side(far_same_label.image.to_algebraic_vector()));
    assert(not (h.on_positive_side(other_side_same_label.image.to_algebraic_vector())));
    assert(not (h.on_positive_side(other_side_different_label.image.to_algebraic_vector())));

};

