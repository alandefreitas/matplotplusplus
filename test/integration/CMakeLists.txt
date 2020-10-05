cmake_minimum_required(VERSION 3.14)
project(myproject VERSION 1.0.1)
set(CMAKE_CXX_STANDARD 17)

find_package(Matplot++)

if (Matplot++_FOUND)
    add_executable(myprogram main.cpp)
    target_link_libraries(myprogram PRIVATE Matplot++::matplot)
endif()