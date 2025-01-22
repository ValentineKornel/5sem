cmake_minimum_required(VERSION 3.20)

include_directories(${SOEx_SOURCE_DIR}/LibProject/include)
add_library(Mappingd SHARED Mappingd.c)