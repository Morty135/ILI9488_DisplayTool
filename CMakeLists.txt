cmake_minimum_required(VERSION 3.10)
project(ILI948_DisplayTool)

set(CMAKE_CXX_STANDARD 20)

# Add executable
add_executable("${CMAKE_PROJECT_NAME}" main.cpp)

# Link WiringPi library
find_package(PkgConfig REQUIRED)
pkg_check_modules(WIRINGPI REQUIRED wiringPi)
include_directories(${WIRINGPI_INCLUDE_DIRS})
link_directories(${WIRINGPI_LIBRARY_DIRS})
target_link_libraries("${CMAKE_PROJECT_NAME}" ${WIRINGPI_LIBRARIES})