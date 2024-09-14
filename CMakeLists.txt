cmake_minimum_required(VERSION 3.29)
project(ILI948_DisplayTool)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES "src/*")
file(GLOB HEADERS "headers/*")

# Add executable
add_executable("${CMAKE_PROJECT_NAME}"  ${SOURCES} ${HEADERS})


include_directories(${WIRINGPI_INCLUDE_DIRS})

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/headers")

link_directories(${WIRINGPI_LIBRARY_DIRS})
target_link_libraries("${CMAKE_PROJECT_NAME}" ${WIRINGPI_LIBRARIES})