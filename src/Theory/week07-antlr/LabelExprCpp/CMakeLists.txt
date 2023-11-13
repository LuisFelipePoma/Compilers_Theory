cmake_minimum_required(VERSION 3.20)

project(antlr)

set(ANTRL4I "/usr/include/antlr4-runtime")
file(GLOB LIB_SOURCES "libs/*.cpp" "libs/*.h")

set(CMAKE_CXX_STANDARD 17)

add_executable(antlr  
    main.cpp
    ${LIB_SOURCES}
    
)


# Configura las opciones de compilación
target_include_directories(antlr PRIVATE ${ANTRL4I} libs)

# Enlaza con las bibliotecas necesarias
target_link_libraries(antlr PRIVATE antlr4-runtime stdc++)