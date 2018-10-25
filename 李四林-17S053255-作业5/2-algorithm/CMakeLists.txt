cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(cpp_example)
add_compile_options(-std=c++14)

# opencv lib
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(algorithm algorithm.cpp)
target_link_libraries(algorithm ${OpenCV_LIBS})

##########################################################
# 自定义命令
##########################################################
add_custom_target (clean-all COMMAND rm -rf ${CMAKE_BINARY_DIR}/*)
