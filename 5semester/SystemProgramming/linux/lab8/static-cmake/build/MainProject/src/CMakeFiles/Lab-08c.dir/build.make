# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/valentine/labs/sp/lab8/static-cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/valentine/labs/sp/lab8/static-cmake/build

# Include any dependencies generated for this target.
include MainProject/src/CMakeFiles/Lab-08c.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include MainProject/src/CMakeFiles/Lab-08c.dir/compiler_depend.make

# Include the progress variables for this target.
include MainProject/src/CMakeFiles/Lab-08c.dir/progress.make

# Include the compile flags for this target's objects.
include MainProject/src/CMakeFiles/Lab-08c.dir/flags.make

MainProject/src/CMakeFiles/Lab-08c.dir/Lab-08c.c.o: MainProject/src/CMakeFiles/Lab-08c.dir/flags.make
MainProject/src/CMakeFiles/Lab-08c.dir/Lab-08c.c.o: /home/valentine/labs/sp/lab8/static-cmake/MainProject/src/Lab-08c.c
MainProject/src/CMakeFiles/Lab-08c.dir/Lab-08c.c.o: MainProject/src/CMakeFiles/Lab-08c.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/valentine/labs/sp/lab8/static-cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object MainProject/src/CMakeFiles/Lab-08c.dir/Lab-08c.c.o"
	cd /home/valentine/labs/sp/lab8/static-cmake/build/MainProject/src && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT MainProject/src/CMakeFiles/Lab-08c.dir/Lab-08c.c.o -MF CMakeFiles/Lab-08c.dir/Lab-08c.c.o.d -o CMakeFiles/Lab-08c.dir/Lab-08c.c.o -c /home/valentine/labs/sp/lab8/static-cmake/MainProject/src/Lab-08c.c

MainProject/src/CMakeFiles/Lab-08c.dir/Lab-08c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Lab-08c.dir/Lab-08c.c.i"
	cd /home/valentine/labs/sp/lab8/static-cmake/build/MainProject/src && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/valentine/labs/sp/lab8/static-cmake/MainProject/src/Lab-08c.c > CMakeFiles/Lab-08c.dir/Lab-08c.c.i

MainProject/src/CMakeFiles/Lab-08c.dir/Lab-08c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Lab-08c.dir/Lab-08c.c.s"
	cd /home/valentine/labs/sp/lab8/static-cmake/build/MainProject/src && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/valentine/labs/sp/lab8/static-cmake/MainProject/src/Lab-08c.c -o CMakeFiles/Lab-08c.dir/Lab-08c.c.s

# Object files for target Lab-08c
Lab__08c_OBJECTS = \
"CMakeFiles/Lab-08c.dir/Lab-08c.c.o"

# External object files for target Lab-08c
Lab__08c_EXTERNAL_OBJECTS =

MainProject/src/Lab-08c: MainProject/src/CMakeFiles/Lab-08c.dir/Lab-08c.c.o
MainProject/src/Lab-08c: MainProject/src/CMakeFiles/Lab-08c.dir/build.make
MainProject/src/Lab-08c: LibProject/src/libLibProject.a
MainProject/src/Lab-08c: MainProject/src/CMakeFiles/Lab-08c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/valentine/labs/sp/lab8/static-cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Lab-08c"
	cd /home/valentine/labs/sp/lab8/static-cmake/build/MainProject/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Lab-08c.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
MainProject/src/CMakeFiles/Lab-08c.dir/build: MainProject/src/Lab-08c
.PHONY : MainProject/src/CMakeFiles/Lab-08c.dir/build

MainProject/src/CMakeFiles/Lab-08c.dir/clean:
	cd /home/valentine/labs/sp/lab8/static-cmake/build/MainProject/src && $(CMAKE_COMMAND) -P CMakeFiles/Lab-08c.dir/cmake_clean.cmake
.PHONY : MainProject/src/CMakeFiles/Lab-08c.dir/clean

MainProject/src/CMakeFiles/Lab-08c.dir/depend:
	cd /home/valentine/labs/sp/lab8/static-cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/valentine/labs/sp/lab8/static-cmake /home/valentine/labs/sp/lab8/static-cmake/MainProject/src /home/valentine/labs/sp/lab8/static-cmake/build /home/valentine/labs/sp/lab8/static-cmake/build/MainProject/src /home/valentine/labs/sp/lab8/static-cmake/build/MainProject/src/CMakeFiles/Lab-08c.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : MainProject/src/CMakeFiles/Lab-08c.dir/depend

