add_executable(main main.cpp)
target_link_libraries(main 
    PRIVATE
    glad
    glfw
) 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # prevents error squiggles for SOURCE_DIR
target_compile_definitions(main 
    PRIVATE
    SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)