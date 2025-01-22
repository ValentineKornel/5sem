cmake_minimum_required(VERSION 3.20)

include_directories(${Lab08_SOURCE_DIR}/LibProject/include)
add_library(LibProject STATIC Mappings.c)