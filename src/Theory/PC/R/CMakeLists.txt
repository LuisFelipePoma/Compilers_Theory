cmake_minimum_required(VERSION 3.14)

project(prog)
include_directories($ENV{ANTLR4I} $ENV{LLVM17I})
file(GLOB SOURCES "*.cpp" "libs/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_directories(${PROJECT_NAME} PUBLIC $ENV{ANTLR4L} $ENV{LLVM17L})
target_link_libraries(${PROJECT_NAME} antlr4-runtime LLVM-17)