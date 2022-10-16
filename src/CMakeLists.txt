cmake_minimum_required(VERSION 3.21)
project(perceptron)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -DEIGEN_STACK_ALLOCATION_LIMIT=0")

add_executable(perceptron main.cpp perceptron.h Image.cpp Pattern.cpp Dataset.cpp readers.cpp PerceptronNetwork.cpp hyperplane.h Hyperplane.cpp Perceptron.cpp utils.cpp utils.h test.cpp test.h)
