#include "perceptron.h"

Image::Image(const uint8_t* bytes)
:
pixels{0}
{
    for(uint32_t row = 0; row < IMAGE_HEIGHT; ++row){
        for(uint32_t column = 0; column < IMAGE_WIDTH; ++column){
            pixels[IMAGE_WIDTH * row + column] = bytes[IMAGE_WIDTH * row + column];
        }
    }
}

double Image::euclidean_distance_squared(const Image &that) const {
    double result = 0;
    double temp;
    for(uint32_t i = 0; i < IMAGE_SIZE; ++i){
        temp = (this->pixels[i] - that.pixels[i]);
        result += temp*temp;
    }
    return result;
}

bool Image::operator==(const Image& that) const{
    for(uint32_t i = 0; i < IMAGE_SIZE; ++i){
        if(this->pixels[i] != that.pixels[i]) return false;
    }
    return true;
}

vector<double> Image::to_algebraic_vector() const {
    auto res = vector<double>();
    for(unsigned char pixel : pixels){
        res.push_back(pixel);
    }
    return res;
}


