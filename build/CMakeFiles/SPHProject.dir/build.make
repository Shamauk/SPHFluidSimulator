# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lovelace/Documents/McGill/sem6/COMP559/project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lovelace/Documents/McGill/sem6/COMP559/project/build

# Include any dependencies generated for this target.
include CMakeFiles/SPHProject.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/SPHProject.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/SPHProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SPHProject.dir/flags.make

CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.o: CMakeFiles/SPHProject.dir/flags.make
CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.o: /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/fluidSimulator.cpp
CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.o: CMakeFiles/SPHProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lovelace/Documents/McGill/sem6/COMP559/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.o -MF CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.o.d -o CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.o -c /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/fluidSimulator.cpp

CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/fluidSimulator.cpp > CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.i

CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/fluidSimulator.cpp -o CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.s

CMakeFiles/SPHProject.dir/src/kernel.cpp.o: CMakeFiles/SPHProject.dir/flags.make
CMakeFiles/SPHProject.dir/src/kernel.cpp.o: /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/kernel.cpp
CMakeFiles/SPHProject.dir/src/kernel.cpp.o: CMakeFiles/SPHProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lovelace/Documents/McGill/sem6/COMP559/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/SPHProject.dir/src/kernel.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SPHProject.dir/src/kernel.cpp.o -MF CMakeFiles/SPHProject.dir/src/kernel.cpp.o.d -o CMakeFiles/SPHProject.dir/src/kernel.cpp.o -c /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/kernel.cpp

CMakeFiles/SPHProject.dir/src/kernel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SPHProject.dir/src/kernel.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/kernel.cpp > CMakeFiles/SPHProject.dir/src/kernel.cpp.i

CMakeFiles/SPHProject.dir/src/kernel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SPHProject.dir/src/kernel.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/kernel.cpp -o CMakeFiles/SPHProject.dir/src/kernel.cpp.s

CMakeFiles/SPHProject.dir/src/main.cpp.o: CMakeFiles/SPHProject.dir/flags.make
CMakeFiles/SPHProject.dir/src/main.cpp.o: /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/main.cpp
CMakeFiles/SPHProject.dir/src/main.cpp.o: CMakeFiles/SPHProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lovelace/Documents/McGill/sem6/COMP559/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/SPHProject.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SPHProject.dir/src/main.cpp.o -MF CMakeFiles/SPHProject.dir/src/main.cpp.o.d -o CMakeFiles/SPHProject.dir/src/main.cpp.o -c /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/main.cpp

CMakeFiles/SPHProject.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SPHProject.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/main.cpp > CMakeFiles/SPHProject.dir/src/main.cpp.i

CMakeFiles/SPHProject.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SPHProject.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/main.cpp -o CMakeFiles/SPHProject.dir/src/main.cpp.s

CMakeFiles/SPHProject.dir/src/particle.cpp.o: CMakeFiles/SPHProject.dir/flags.make
CMakeFiles/SPHProject.dir/src/particle.cpp.o: /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/particle.cpp
CMakeFiles/SPHProject.dir/src/particle.cpp.o: CMakeFiles/SPHProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lovelace/Documents/McGill/sem6/COMP559/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/SPHProject.dir/src/particle.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SPHProject.dir/src/particle.cpp.o -MF CMakeFiles/SPHProject.dir/src/particle.cpp.o.d -o CMakeFiles/SPHProject.dir/src/particle.cpp.o -c /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/particle.cpp

CMakeFiles/SPHProject.dir/src/particle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SPHProject.dir/src/particle.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/particle.cpp > CMakeFiles/SPHProject.dir/src/particle.cpp.i

CMakeFiles/SPHProject.dir/src/particle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SPHProject.dir/src/particle.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lovelace/Documents/McGill/sem6/COMP559/project/src/particle.cpp -o CMakeFiles/SPHProject.dir/src/particle.cpp.s

# Object files for target SPHProject
SPHProject_OBJECTS = \
"CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.o" \
"CMakeFiles/SPHProject.dir/src/kernel.cpp.o" \
"CMakeFiles/SPHProject.dir/src/main.cpp.o" \
"CMakeFiles/SPHProject.dir/src/particle.cpp.o"

# External object files for target SPHProject
SPHProject_EXTERNAL_OBJECTS =

SPHProject: CMakeFiles/SPHProject.dir/src/fluidSimulator.cpp.o
SPHProject: CMakeFiles/SPHProject.dir/src/kernel.cpp.o
SPHProject: CMakeFiles/SPHProject.dir/src/main.cpp.o
SPHProject: CMakeFiles/SPHProject.dir/src/particle.cpp.o
SPHProject: CMakeFiles/SPHProject.dir/build.make
SPHProject: /usr/local/lib/glfw-3.3.8/debug/src/libglfw3.a
SPHProject: /usr/local/lib/glew-2.1.0/lib/libGLEW.a
SPHProject: CMakeFiles/SPHProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/lovelace/Documents/McGill/sem6/COMP559/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable SPHProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SPHProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SPHProject.dir/build: SPHProject
.PHONY : CMakeFiles/SPHProject.dir/build

CMakeFiles/SPHProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SPHProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SPHProject.dir/clean

CMakeFiles/SPHProject.dir/depend:
	cd /Users/lovelace/Documents/McGill/sem6/COMP559/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lovelace/Documents/McGill/sem6/COMP559/project /Users/lovelace/Documents/McGill/sem6/COMP559/project /Users/lovelace/Documents/McGill/sem6/COMP559/project/build /Users/lovelace/Documents/McGill/sem6/COMP559/project/build /Users/lovelace/Documents/McGill/sem6/COMP559/project/build/CMakeFiles/SPHProject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SPHProject.dir/depend

