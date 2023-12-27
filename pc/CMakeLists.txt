cmake_minimum_required(VERSION 3.0)
project(yage-pc)

set(CMAKE_CXX_STANDARD 20)

add_executable(yage-pc pc_main.cc)

add_subdirectory(glfw)
find_package(glfw)
find_package(OpenGL REQUIRED)

target_include_directories(yage-pc PRIVATE ../engine)
target_link_libraries(yage-pc
        PRIVATE
        yage-engine
        OpenGL::GL
        glfw
)

