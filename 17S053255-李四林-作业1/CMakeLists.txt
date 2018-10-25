cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(learning_opencv)
add_compile_options(-std=c++14)
# 换个地方保存目标二进制
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# opencv lib
set(OpenCV_DIR "/opt/ros/kinetic/share/OpenCV-3.3.1-dev")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#include
include_directories(include)

add_executable(homework src/main.cpp src/draw_gui.cpp include/control_plane.h src/algorithm.cpp)
target_link_libraries(homework ${OpenCV_LIBS})

add_executable(cvui_test src/cvui_test.cpp)
target_link_libraries(cvui_test ${OpenCV_LIBS})

##########################################################
# 测试
##########################################################
# 启用测试
# enable_testing()

# # 测试程序是否成功运行
# add_test (test_run demo)

# # 测试帮助信息是否可以正常提示
# # PASS_REGULAR_EXPRESSION 用来测试输出是否包含后面跟着的字符串
# add_test (test_usage demo)
# set_tests_properties (test_usage
#   PROPERTIES PASS_REGULAR_EXPRESSION ".*")

# # 定义一个宏，用来简化测试工作
# macro (do_test arg1 arg2 result)
#   add_test (test_${arg1}_${arg2} demo ${arg1} ${arg2})
#   set_tests_properties (test_${arg1}_${arg2}
#     PROPERTIES PASS_REGULAR_EXPRESSION ${result})
# endmacro (do_test)

# # 利用 do_test 宏，测试一系列数据
# # do_test ()



##########################################################
# 自定义命令
##########################################################
# Make clean-all
add_custom_target (clean-all COMMAND rm -rf ${CMAKE_BINARY_DIR}/*)
