cmake_minimum_required(VERSION 3.1)
project(test)

include(CMakeFiles/conan_toolchain.cmake)

cmake_policy(SET CMP0091 NEW)

find_package(bcryptcpp REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} bcryptcpp::bcryptcpp)