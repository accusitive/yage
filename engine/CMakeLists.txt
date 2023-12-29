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
SET(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(box2d)

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
target_link_libraries(yage-engine box2d)

#include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/shaders.cmake)
#set(SHDC "${CMAKE_SOURCE_DIR}/engine/sokol-tools-bin/bin/linux/sokol-shdc")
#add_custom_target(compile_shaders ALL COMMAND ${SHDC} "--input" "${CMAKE_SOURCE_DIR}/assets/shaders/f_triangle.glsl" "--output" "${CMAKE_SOURCE_DIR}/engine/gen/f_triangle.glsl.h" "--slang" "glsl330"
#        COMMENT "Compiling shaders..." VERBATIM)

#add_dependencies(yage-engine compile_shaders)
set(SHDC "${CMAKE_SOURCE_DIR}/engine/sokol-tools-bin/bin/linux/sokol-shdc")

function(compile_shader SHADER_INPUT SHADER_OUTPUT SLANG)
    execute_process(
            COMMAND ${SHDC} "--input" "${CMAKE_SOURCE_DIR}/assets/shaders/${SHADER_INPUT}" "--output" "${CMAKE_SOURCE_DIR}/engine/gen/${SHADER_OUTPUT}" "--slang" "${SLANG}"
            RESULT_VARIABLE result
    )
    if(result)
        message(FATAL_ERROR "Failed to compile shader.")
    endif()
endfunction()
compile_shader("triangle.glsl" "triangle.glsl.h" "glsl330")

target_include_directories(yage-engine PUBLIC
        #        ${squirrel_SOURCE_DIR}/include
        #        ${squall_SOURCE_DIR}
        #        ${chipmunk_SOURCE_DIR}/include
        box2d/include
        HandmadeMath/
        ${imgui_SOURCE_DIR}
)
