cmake_minimum_required(VERSION 3.30)
project(sorting)

set(CMAKE_CXX_STANDARD 20)

add_executable(sorting main.cpp
        src/include/path.h
        src/path.cpp
        src/include/sorting.h
        src/include/licensePlate.h
        src/include/personName.h
        src/include/generator.h
        src/include/sortingType.h
)
