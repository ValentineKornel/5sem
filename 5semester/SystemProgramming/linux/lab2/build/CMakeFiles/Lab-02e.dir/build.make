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
CMAKE_SOURCE_DIR = /home/valentine/labs/sp/lab2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/valentine/labs/sp/lab2/build

# Include any dependencies generated for this target.
include CMakeFiles/Lab-02e.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Lab-02e.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Lab-02e.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Lab-02e.dir/flags.make

CMakeFiles/Lab-02e.dir/Lab-02e.c.o: CMakeFiles/Lab-02e.dir/flags.make
CMakeFiles/Lab-02e.dir/Lab-02e.c.o: /home/valentine/labs/sp/lab2/Lab-02e.c
CMakeFiles/Lab-02e.dir/Lab-02e.c.o: CMakeFiles/Lab-02e.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/valentine/labs/sp/lab2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Lab-02e.dir/Lab-02e.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Lab-02e.dir/Lab-02e.c.o -MF CMakeFiles/Lab-02e.dir/Lab-02e.c.o.d -o CMakeFiles/Lab-02e.dir/Lab-02e.c.o -c /home/valentine/labs/sp/lab2/Lab-02e.c

CMakeFiles/Lab-02e.dir/Lab-02e.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Lab-02e.dir/Lab-02e.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/valentine/labs/sp/lab2/Lab-02e.c > CMakeFiles/Lab-02e.dir/Lab-02e.c.i

CMakeFiles/Lab-02e.dir/Lab-02e.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Lab-02e.dir/Lab-02e.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/valentine/labs/sp/lab2/Lab-02e.c -o CMakeFiles/Lab-02e.dir/Lab-02e.c.s

# Object files for target Lab-02e
Lab__02e_OBJECTS = \
"CMakeFiles/Lab-02e.dir/Lab-02e.c.o"

# External object files for target Lab-02e
Lab__02e_EXTERNAL_OBJECTS =

Lab-02e: CMakeFiles/Lab-02e.dir/Lab-02e.c.o
Lab-02e: CMakeFiles/Lab-02e.dir/build.make
Lab-02e: CMakeFiles/Lab-02e.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/valentine/labs/sp/lab2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Lab-02e"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Lab-02e.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Lab-02e.dir/build: Lab-02e
.PHONY : CMakeFiles/Lab-02e.dir/build

CMakeFiles/Lab-02e.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Lab-02e.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Lab-02e.dir/clean

CMakeFiles/Lab-02e.dir/depend:
	cd /home/valentine/labs/sp/lab2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/valentine/labs/sp/lab2 /home/valentine/labs/sp/lab2 /home/valentine/labs/sp/lab2/build /home/valentine/labs/sp/lab2/build /home/valentine/labs/sp/lab2/build/CMakeFiles/Lab-02e.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Lab-02e.dir/depend

