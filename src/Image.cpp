#include "perceptron.h"

Image::Image(const uint8_t* bytes)
:
pixels{0}
{
    double max = 0;
    for(uint32_t row = 0; row < IMAGE_HEIGHT; ++row){
        for(uint32_t column = 0; column < IMAGE_WIDTH; ++column){
            auto new_element = (double) bytes[IMAGE_WIDTH * row + column];
            pixels[IMAGE_WIDTH * row + column] = new_element;
            if(new_element > max){
                    max = new_element;
            }
        }
    }
    for(double & pixel : pixels){
        // Normalizing to range[-1, 1] for better algebraic properties
        if(pixel != 0.0){
            pixel = pixel / max * 2.0 - 1.0;
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

vector<double> Image::create_algebraic_vector() const{
    auto res = vector<double>();
    for(double pixel : pixels){
        res.push_back(pixel);
    }
    return res;
}

vector<double> Image::to_algebraic_vector() const {
    static auto res = this->create_algebraic_vector();
    return res;
}


