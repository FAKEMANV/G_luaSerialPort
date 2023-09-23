cmake_minimum_required (VERSION 3.8)

project ("liblua" LANGUAGES C )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${PROJECT_NAME} STATIC ${src})