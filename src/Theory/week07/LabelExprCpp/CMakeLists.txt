cmake_minimum_required(VERSION 3.20)

project(antlr)
add_executable(antlr  
    main.cpp
    libs/LabeledExprBaseVisitor.cpp
    libs/LabeledExprVisitor.cpp
    libs/LabeledExprParser.cpp
    libs/LabeledExprLexer.cpp
    
)

target_link_libraries(antlr PRIVATE /home/felipe/Disk/Apps/antlr4-cpp/runtime/src/antlr4-runtime.h)
