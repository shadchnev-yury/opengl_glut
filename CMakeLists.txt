cmake_minimum_required(VERSION 3.28.0)

project(cmake_opengl VERSION 1.0.0)

find_library(COCOA Cocoa)
find_library(COREVIDEO CoreVideo)
find_library(IOKIT IOKit)
find_library(OPENGL OpenGl)
find_library(GLUT GLUT)

if (NOT COCOA OR NOT COREVIDEO OR NOT IOKIT OR NOT GLUT)
    message(FATAL_ERROR "Cocoa|CoreVideo|IOKit|GLUT not found")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(GL_SILENCE_DEPRECATION)

add_executable(cmake_opengl 
    src/main.cpp
    src/config.h
    src/triangle_mesh.h src/triangle_mesh.cpp
)

target_include_directories(cmake_opengl PRIVATE dependencies)
target_link_directories(cmake_opengl PRIVATE libs)

target_link_libraries(cmake_opengl PRIVATE ${OPENGL} ${COCOA} ${COREVIDEO} ${IOKIT} ${GLUT})

enable_testing()