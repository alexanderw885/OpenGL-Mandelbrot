add_executable(main main.cpp)
target_link_libraries(main 
    PRIVATE
    glad
    glfw
    stb
    rapidjson
) 

target_compile_definitions(main
    PRIVATE
    ASSET_PATH="${CMAKE_SOURCE_DIR}/assets/"
    OUTPUT_PATH="${CMAKE_SOURCE_DIR}/output/"
    CFG_PATH="${CMAKE_SOURCE_DIR}/config/"
)