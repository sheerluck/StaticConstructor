cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(StaticConstructor LANGUAGES CXX)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    find_package(GTest)
    if(GTEST_FOUND)
        enable_testing()
    endif()
endif()

include_directories(include)
add_subdirectory(src)
