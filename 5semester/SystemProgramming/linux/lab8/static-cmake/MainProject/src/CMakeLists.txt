cmake_minimum_required(VERSION 3.20)

include_directories(${Lab08_SOURCE_DIR}/LibProject/include)
link_directories(${Lab08_SOURCE_DIR}/LibProject/src)
add_executable(Lab-08c Lab-08c.c)
target_link_libraries(Lab-08c LibProject)