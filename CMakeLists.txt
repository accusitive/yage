cmake_minimum_required(VERSION 3.0)
project(yage)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_EXAMPLES off)

include(FetchContent)

FetchContent_Declare(
        squall
        URL      https://github.com/jonigata/squall/archive/master.zip
)

FetchContent_MakeAvailable(squall)


message([TRACE] " squall_SOURCE_DIR = ${squall_SOURCE_DIR}/squall  ")

add_executable(yage main.cpp)
target_link_libraries ( yage squirrel_static sqstdlib_static)
target_include_directories ( yage PUBLIC
        ${squirrel_SOURCE_DIR}/include
        ${squall_SOURCE_DIR}
)

#add_executable ( squirrel-test squirrel_test.cpp)
