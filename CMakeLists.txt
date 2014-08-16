cmake_minimum_required(VERSION 2.6)
project(TwinklebearDevLessons)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${TwinklebearDevLessons_SOURCE_DIR}/cmake")
set(BIN_DIR ${TwinklebearDevLessons_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${TwinklebearDevLessons_SOURCE_DIR}/include)

add_subdirectory(Lesson2)
