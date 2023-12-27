cmake_minimum_required(VERSION 3.0)
project(yage-project)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_EXAMPLES off)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(SOKOL_HEADERS
        sokol/sokol_gfx.h
        sokol/sokol_app.h
        sokol_imgui.h
        sokol/sokol_glue.h)

include(FetchContent)

cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0072 NEW)

FetchContent_Declare(
        squall
        URL https://github.com/jonigata/squall/archive/master.zip
)
FetchContent_Declare(
        chipmunk
        URL https://github.com/slembcke/Chipmunk2D/archive/master.zip
)
FetchContent_Declare(
        flecs
        URL https://github.com/SanderMertens/flecs/archive/master.zip
)
FetchContent_MakeAvailable(squall)
FetchContent_MakeAvailable(chipmunk)
FetchContent_MakeAvailable(flecs)


# IMGUI
add_library(imgui STATIC
        imgui/imgui.h
        imgui/imgui_internal.h
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp)

target_include_directories(imgui PUBLIC
        imgui/
        imgui/backends)
add_subdirectory(glfw)
find_package(glfw)
find_package(OpenGL REQUIRED)

message([TRACE] " squall_SOURCE_DIR = ${squall_SOURCE_DIR}/squall  ")
message([TRACE] " chipmunk_SOURCE_DIR = ${chipmunk_SOURCE_DIR}/  ")

add_library(yage-engine
        engine/engine.cc
        engine/engine.hh)

target_link_libraries(yage-engine squirrel_static sqstdlib_static)
target_link_libraries(yage-engine flecs::flecs_static)
target_link_libraries(yage-engine chipmunk)
target_link_libraries(yage-engine OpenGL::GL)
target_link_libraries(yage-engine imgui)

target_include_directories(yage-engine PUBLIC
        ${squirrel_SOURCE_DIR}/include
        ${squall_SOURCE_DIR}
        ${chipmunk_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}
)

add_executable(yage-pc pc/main.cc)
target_link_libraries(yage-pc yage-engine)
target_link_libraries(yage-pc glfw)

target_include_directories(yage-pc PUBLIC engine)
