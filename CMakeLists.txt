cmake_minimum_required(VERSION 3.22.1)
project(Gina)

include_directories(${CMAKE_SOURCE_DIR}/include )

add_subdirectory(include/glfw)
add_subdirectory(include/glm)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/cpp/gina.cpp src/cpp/glad.c)

# Copy shader files to the build directory
configure_file(src/cpp/shader.vs ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/cpp/shader.fs ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/cpp/lightshader.fs ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# Add shader source files to the project
target_sources(${PROJECT_NAME} PRIVATE
    ./src/cpp/shader.vs
    ./src/cpp/shader.fs
    ./src/cpp/lightshader.fs
)

target_include_directories(${PROJECT_NAME}
    PUBLIC include/glfw/include/GLFW
    PUBLIC include/glm
)

target_link_directories(${PROJECT_NAME}
    PRIVATE include/glfw/src/Debug
    PRIVATE include/glm/src/Debug
)

target_link_libraries(${PROJECT_NAME} glfw glm)
