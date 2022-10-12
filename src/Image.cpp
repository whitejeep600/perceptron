#include "perceptron.h"

Image::Image(const uint8_t* bytes)
:
pixels{{0}}
{
    for(uint32_t row = 0; row < IMAGE_HEIGHT; ++row){
        for(uint32_t column = 0; column < IMAGE_WIDTH; ++column){
            pixels[row][column] = bytes[IMAGE_WIDTH * row + column];
        }
    }
}
