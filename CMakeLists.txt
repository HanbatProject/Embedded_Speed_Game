cmake_minimum_required(VERSION 3.7)
project(EmbeddedSpeedGame)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.cpp dot_matrix.cpp keypad.cpp print_text.cpp external_includes.h functions.h machine.h state.h text.h actions.cpp)
add_library(EmbeddedSpeedGame ${SOURCE_FILES})
