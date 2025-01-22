создать файл CmakeLists.txt с содерижмым:

cmake_minimum_required(VERSION 3.10)
project(WordsCounter)

set(CMAKE_CXX_STANDARD 11)

add_executable(CountWords example.c)

Настроить Cmake с использованием этого файла

mkdir build
cd build
CC=clang CXX=clang++ cmake ..

make
