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

# Utility rule file for Sphinx.

# Include the progress variables for this target.
include docs/CMakeFiles/Sphinx.dir/progress.make

docs/CMakeFiles/Sphinx:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir="/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Generating documentation with Sphinx"
	cd "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/docs" && /usr/bin/sphinx-build -b html -Dbreathe_projects.Anchors=/mnt/c/Users/user/Google\ Drive/CodeProjects/anchors/cmake-build-debug/docs/docs/doxygen/xml /mnt/c/Users/user/Google\ Drive/CodeProjects/anchors/docs /mnt/c/Users/user/Google\ Drive/CodeProjects/anchors/cmake-build-debug/docs/docs/sphinx

Sphinx: docs/CMakeFiles/Sphinx
Sphinx: docs/CMakeFiles/Sphinx.dir/build.make

.PHONY : Sphinx

# Rule to build all files generated by this target.
docs/CMakeFiles/Sphinx.dir/build: Sphinx

.PHONY : docs/CMakeFiles/Sphinx.dir/build

docs/CMakeFiles/Sphinx.dir/clean:
	cd "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/docs" && $(CMAKE_COMMAND) -P CMakeFiles/Sphinx.dir/cmake_clean.cmake
.PHONY : docs/CMakeFiles/Sphinx.dir/clean

docs/CMakeFiles/Sphinx.dir/depend:
	cd "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/docs" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/docs" "/mnt/c/Users/user/Google Drive/CodeProjects/anchors/cmake-build-debug/docs/CMakeFiles/Sphinx.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : docs/CMakeFiles/Sphinx.dir/depend

