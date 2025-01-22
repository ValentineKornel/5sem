cmake_minimum_required(VERSION 3.10)

project(Lab9)

include_directories(/home/valentine/.local/share/Trash/files)

set(SOURCES
    auto_daemon.c
    become_daemon.c
)

add_executable(Service09d ${SOURCES})