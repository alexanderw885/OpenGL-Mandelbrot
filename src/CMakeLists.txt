add_executable(main main.cpp)
target_link_libraries(main 
    PRIVATE
    glad
    glfw
    stb
) 

target_compile_definitions(main
    PRIVATE
    ASSET_PATH="${CMAKE_SOURCE_DIR}/assets/"
)