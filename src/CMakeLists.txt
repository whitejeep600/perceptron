cmake_minimum_required(VERSION 3.21)
project(perceptron)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

add_executable(perceptron main.cpp perceptron.h Image.cpp Sample.cpp Dataset.cpp readers.cpp)
