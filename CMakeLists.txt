cmake_minimum_required(VERSION 3.28)

add_subdirectory(libs)
add_subdirectory(src)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # prevents error squiggles for compile definitions
