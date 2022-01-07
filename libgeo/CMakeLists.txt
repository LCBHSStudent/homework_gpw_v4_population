cmake_minimum_required(VERSION 3.20)
project(libgeo)

add_compile_options(-Wall)
add_compile_options(-Wno-strict-aliasing)
add_compile_options(-Wno-unused-function)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g --coverage -fprofile-arcs -ftest-coverage -fPIC -O0")

include_directories(include)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
find_package(pybind11 REQUIRED)

file(GLOB_RECURSE LIB_HEADER include/libgeo/*.h)
file(GLOB_RECURSE LIB_SOURCE src/libgeo/*.cpp)

pybind11_add_module(libgeo SHARED src/pybinding.cpp ${LIB_SOURCE})
target_link_libraries(libgeo PRIVATE ${PYTHON_LIBRARIES})

add_executable(test src/test_main.cpp)
target_link_libraries(test PRIVATE libgeo)
