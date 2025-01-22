cmake_minimum_required(VERSION 3.20)

include_directories(${Lab-08b_SOURCE_DIR}/LibProject/include)
link_directories(${Lab-08b_SOURCE_DIR}/LibProject/src)
add_executable(Inj Lab-08b.c)