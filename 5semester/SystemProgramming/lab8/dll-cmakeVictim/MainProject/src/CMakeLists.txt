cmake_minimum_required(VERSION 3.20)

include_directories(${Lab-08b_SOURCE_DIR}/LibMappingd2Project/include)
include_directories(${Lab-08b_SOURCE_DIR}/LibMappingdProject/include)
link_directories(${Lab-08b_SOURCE_DIR}/LibMappingd2Project/src)
link_directories(${Lab-08b_SOURCE_DIR}/LibMappingdProject/src)
add_executable(Lab-08b Lab-08b.c)
target_link_libraries(Lab-08b Mappingd)
target_link_libraries(Lab-08b Mappingd-2)
