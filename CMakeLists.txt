cmake_minimum_required(VERSION 3.0)
project(yage)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_EXAMPLES off)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include(FetchContent)


FetchContent_Declare(
        squall
        URL      https://github.com/jonigata/squall/archive/master.zip
)

FetchContent_MakeAvailable(squall)
add_subdirectory(flecs)
add_subdirectory(glfw)
find_package(OpenGL REQUIRED)

message([TRACE] " squall_SOURCE_DIR = ${squall_SOURCE_DIR}/squall  ")

# set(SOKOL_HEADERS
#         sokol/sokol_gfx.h
#         sokol/sokol_app.h
#         sokol/sokol_audio.h
#         sokol/sokol_time.h
#         sokol/sokol_glue.h)
# add_library(sokol STATIC sokol.c ${SOKOL_HEADERS})

add_executable(yage main.cpp
        Engine.cc
        Engine.hh
        )
target_link_libraries(yage squirrel_static sqstdlib_static)
target_link_libraries(yage flecs::flecs_static)
target_link_libraries(yage glfw)
target_link_libraries(yage OpenGL::GL)

target_include_directories (yage PUBLIC
        ${squirrel_SOURCE_DIR}/include
        ${squall_SOURCE_DIR}
)

#add_executable ( squirrel-test squirrel_test.cpp)
