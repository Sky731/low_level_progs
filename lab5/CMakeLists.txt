cmake_minimum_required(VERSION 3.9)
project(lab5 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab5 main.c bmp_wrapper.c bmp_wrapper.h grand_bmp.c grand_bmp.h)