cmake_minimum_required(VERSION 3.21)
project(yage-project)

set(CMAKE_CXX_STANDARD 20)

set(SOKOL_HEADERS
        ../sokol/sokol_gfx.h
        ../sokol/sokol_app.h
        ../sokol_imgui.h
        ../sokol/sokol_glue.h)

include(FetchContent)

cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0079 OLD)

add_subdirectory(entt)

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
        imgui
        imgui/backends)

add_library(yage-engine
        source/util.cc
        source/util.hh
        source/engine.cc
        source/engine.hh
        source/resource.cc
        source/resource.hh
)

# Link libraries
target_link_libraries(yage-engine EnTT::EnTT)
target_link_libraries(yage-engine imgui)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/shaders.cmake)

target_include_directories(yage-engine PUBLIC
#        ${squirrel_SOURCE_DIR}/include
#        ${squall_SOURCE_DIR}
#        ${chipmunk_SOURCE_DIR}/include
        HandmadeMath/
        ${imgui_SOURCE_DIR}
)
