cmake_minimum_required(VERSION 3.20)

include_directories(${Lab-08b_SOURCE_DIR}/LibMappingdProject/include)
add_library(Mappingd SHARED Mappingd.c)
target_sources(Mappingd PUBLIC Mappingd.def)