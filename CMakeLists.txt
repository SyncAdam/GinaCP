cmake_minimum_required(VERSION 3.28.1)
project(Gina)

include_directories(${CMAKE_SOURCE_DIR}/include )

add_subdirectory(include/glfw)
add_subdirectory(include/glm)


add_executable(${PROJECT_NAME} src/cpp/gina.cpp src/cpp/glad.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC include/glfw/include/GLFW
    PUBLIC include/glm
)

target_link_directories(${PROJECT_NAME}
    PRIVATE include/glfw/src/Debug
    PRIVATE include/glm/src/Debug
)

target_link_libraries(${PROJECT_NAME} glfw glm)