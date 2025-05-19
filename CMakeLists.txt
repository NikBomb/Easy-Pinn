cmake_minimum_required(VERSION 3.13)  
project(easyPinn)               
set(CMAKE_CXX_STANDARD 20)            


set(SOURCE_FILES main.cpp finiteDifferences.hpp)

add_executable(easyPinn ${SOURCE_FILES})