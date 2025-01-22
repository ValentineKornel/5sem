создать файл toolchain.cmake с содерижмым:

set(CMAKE_C_COMPILER x86_64-w64_mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64_mingw32-g++)

Настроить Cmake с использованием этого файла

mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..

make
