# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/c/Users/user/Google Drive/CodeProjects/anchors"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug"

# Include any dependencies generated for this target.
include src/CMakeFiles/demo.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/demo.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/demo.dir/flags.make

src/CMakeFiles/demo.dir/main.cpp.o: src/CMakeFiles/demo.dir/flags.make
src/CMakeFiles/demo.dir/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/demo.dir/main.cpp.o"
	cd "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/src" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/demo.dir/main.cpp.o -c "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/src/main.cpp"

src/CMakeFiles/demo.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo.dir/main.cpp.i"
	cd "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/src/main.cpp" > CMakeFiles/demo.dir/main.cpp.i

src/CMakeFiles/demo.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo.dir/main.cpp.s"
	cd "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/src/main.cpp" -o CMakeFiles/demo.dir/main.cpp.s

# Object files for target demo
demo_OBJECTS = \
"CMakeFiles/demo.dir/main.cpp.o"

# External object files for target demo
demo_EXTERNAL_OBJECTS =

src/demo: src/CMakeFiles/demo.dir/main.cpp.o
src/demo: src/CMakeFiles/demo.dir/build.make
src/demo: src/libanchors.a
src/demo: src/CMakeFiles/demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable demo"
	cd "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/demo.dir/build: src/demo

.PHONY : src/CMakeFiles/demo.dir/build

src/CMakeFiles/demo.dir/clean:
	cd "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/src" && $(CMAKE_COMMAND) -P CMakeFiles/demo.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/demo.dir/clean

src/CMakeFiles/demo.dir/depend:
	cd "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/src" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/src" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/src/CMakeFiles/demo.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/demo.dir/depend

