cmake_minimum_required(VERSION 3.20)

include_directories(${Lab-08b_SOURCE_DIR}/LibMappingd2Project/include)
add_library(Mappingd-2 SHARED Mappingd-2.c)