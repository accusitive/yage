cmake_minimum_required(VERSION 3.12)
project(yage)

add_subdirectory(engine)
if(SWITCH)
    add_subdirectory(platform_switch)
else()
    add_subdirectory(pc)
endif()