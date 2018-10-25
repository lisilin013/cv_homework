cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(cali_camera)
add_compile_options(-std=c++14)

# opencv lib
set(OpenCV_DIR "/opt/ros/kinetic/share/OpenCV-3.3.1-dev")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories("/usr/include/eigen3")


add_executable(filter filter.cpp)
target_link_libraries(filter ${OpenCV_LIBS})

