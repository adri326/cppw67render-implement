cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(cpp3)

add_executable(cpp3 main.cpp)
target_compile_features(cpp3 PRIVATE cxx_range_for)

add_subdirectory(dependencies/openw67render)
include_directories(dependencies/openw67render/include)

target_link_libraries(cpp3 openw67render soil glfw glbinding ${GLFW_LIBRARIES})

