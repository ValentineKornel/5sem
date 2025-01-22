cmake_minimum_required(VERSION 3.20)

include_directories(${Lab-08b_SOURCE_DIR}/LibProject/include)
add_library(MyLib SHARED MyLib.c)