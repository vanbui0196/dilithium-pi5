# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/vanbu/Desktop/dilithium-pi5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vanbu/Desktop/dilithium-pi5/build

# Include any dependencies generated for this target.
include CMakeFiles/poly_algo_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/poly_algo_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/poly_algo_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/poly_algo_test.dir/flags.make

CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.o: CMakeFiles/poly_algo_test.dir/flags.make
CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.o: /home/vanbu/Desktop/dilithium-pi5/test/test_poly_algo.cpp
CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.o: CMakeFiles/poly_algo_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vanbu/Desktop/dilithium-pi5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.o -MF CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.o.d -o CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.o -c /home/vanbu/Desktop/dilithium-pi5/test/test_poly_algo.cpp

CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vanbu/Desktop/dilithium-pi5/test/test_poly_algo.cpp > CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.i

CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vanbu/Desktop/dilithium-pi5/test/test_poly_algo.cpp -o CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.s

# Object files for target poly_algo_test
poly_algo_test_OBJECTS = \
"CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.o"

# External object files for target poly_algo_test
poly_algo_test_EXTERNAL_OBJECTS =

poly_algo_test: CMakeFiles/poly_algo_test.dir/test/test_poly_algo.cpp.o
poly_algo_test: CMakeFiles/poly_algo_test.dir/build.make
poly_algo_test: libdilithium.a
poly_algo_test: CMakeFiles/poly_algo_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vanbu/Desktop/dilithium-pi5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable poly_algo_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/poly_algo_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/poly_algo_test.dir/build: poly_algo_test
.PHONY : CMakeFiles/poly_algo_test.dir/build

CMakeFiles/poly_algo_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/poly_algo_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/poly_algo_test.dir/clean

CMakeFiles/poly_algo_test.dir/depend:
	cd /home/vanbu/Desktop/dilithium-pi5/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vanbu/Desktop/dilithium-pi5 /home/vanbu/Desktop/dilithium-pi5 /home/vanbu/Desktop/dilithium-pi5/build /home/vanbu/Desktop/dilithium-pi5/build /home/vanbu/Desktop/dilithium-pi5/build/CMakeFiles/poly_algo_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/poly_algo_test.dir/depend

