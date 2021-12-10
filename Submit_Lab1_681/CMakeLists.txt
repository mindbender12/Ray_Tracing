cmake_minimum_required(VERSION 3.0)
  
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

PROJECT(Ray_Tracing)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
include_directories("/home/sdutta/Codes/coin3d/install/include")

find_library(
    COIN3D_LIBRARY
    NAMES Coin
    HINTS "/home/sdutta/Codes/coin3d/install/lib/")

file(GLOB CXX_FILES *.cpp *.h)

add_executable(rt ${CXX_FILES})

target_link_libraries(rt ${COIN3D_LIBRARY})
