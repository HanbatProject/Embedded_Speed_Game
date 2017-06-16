cmake_minimum_required(VERSION 3.7)
project(EmbeddedSpeedGame)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.c dot_matrix.c keypad.c print_text.c external_includes.h functions.h machine.h state.h text.h actions.c thread_functions.h)
add_library(EmbeddedSpeedGame ${SOURCE_FILES})
