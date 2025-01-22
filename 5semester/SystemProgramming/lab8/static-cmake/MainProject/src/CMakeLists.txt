cmake_minimum_required(VERSION 3.20)

include_directories(${Lab8_SOURCE_DIR}/LibProject/include)
link_directories(${Lab8_SOURCE_DIR}/LibProject/src)
add_executable(Lab-08a Lab-08a.c)
target_link_libraries(Lab-08a Mappings)