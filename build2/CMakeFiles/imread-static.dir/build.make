# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.1

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.1.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.1.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/fish/Dropbox/libimread

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/fish/Dropbox/libimread/build2

# Include any dependencies generated for this target.
include CMakeFiles/imread-static.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/imread-static.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/imread-static.dir/flags.make

CMakeFiles/imread-static.dir/src/_bmp.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/_bmp.cpp.o: ../src/_bmp.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/_bmp.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/_bmp.cpp.o -c /Users/fish/Dropbox/libimread/src/_bmp.cpp

CMakeFiles/imread-static.dir/src/_bmp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/_bmp.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/_bmp.cpp > CMakeFiles/imread-static.dir/src/_bmp.cpp.i

CMakeFiles/imread-static.dir/src/_bmp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/_bmp.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/_bmp.cpp -o CMakeFiles/imread-static.dir/src/_bmp.cpp.s

CMakeFiles/imread-static.dir/src/_bmp.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/_bmp.cpp.o.requires

CMakeFiles/imread-static.dir/src/_bmp.cpp.o.provides: CMakeFiles/imread-static.dir/src/_bmp.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/_bmp.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/_bmp.cpp.o.provides

CMakeFiles/imread-static.dir/src/_bmp.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/_bmp.cpp.o

CMakeFiles/imread-static.dir/src/_jpeg.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/_jpeg.cpp.o: ../src/_jpeg.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/_jpeg.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/_jpeg.cpp.o -c /Users/fish/Dropbox/libimread/src/_jpeg.cpp

CMakeFiles/imread-static.dir/src/_jpeg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/_jpeg.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/_jpeg.cpp > CMakeFiles/imread-static.dir/src/_jpeg.cpp.i

CMakeFiles/imread-static.dir/src/_jpeg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/_jpeg.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/_jpeg.cpp -o CMakeFiles/imread-static.dir/src/_jpeg.cpp.s

CMakeFiles/imread-static.dir/src/_jpeg.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/_jpeg.cpp.o.requires

CMakeFiles/imread-static.dir/src/_jpeg.cpp.o.provides: CMakeFiles/imread-static.dir/src/_jpeg.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/_jpeg.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/_jpeg.cpp.o.provides

CMakeFiles/imread-static.dir/src/_jpeg.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/_jpeg.cpp.o

CMakeFiles/imread-static.dir/src/_lsm.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/_lsm.cpp.o: ../src/_lsm.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/_lsm.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/_lsm.cpp.o -c /Users/fish/Dropbox/libimread/src/_lsm.cpp

CMakeFiles/imread-static.dir/src/_lsm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/_lsm.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/_lsm.cpp > CMakeFiles/imread-static.dir/src/_lsm.cpp.i

CMakeFiles/imread-static.dir/src/_lsm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/_lsm.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/_lsm.cpp -o CMakeFiles/imread-static.dir/src/_lsm.cpp.s

CMakeFiles/imread-static.dir/src/_lsm.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/_lsm.cpp.o.requires

CMakeFiles/imread-static.dir/src/_lsm.cpp.o.provides: CMakeFiles/imread-static.dir/src/_lsm.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/_lsm.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/_lsm.cpp.o.provides

CMakeFiles/imread-static.dir/src/_lsm.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/_lsm.cpp.o

CMakeFiles/imread-static.dir/src/_png.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/_png.cpp.o: ../src/_png.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/_png.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/_png.cpp.o -c /Users/fish/Dropbox/libimread/src/_png.cpp

CMakeFiles/imread-static.dir/src/_png.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/_png.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/_png.cpp > CMakeFiles/imread-static.dir/src/_png.cpp.i

CMakeFiles/imread-static.dir/src/_png.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/_png.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/_png.cpp -o CMakeFiles/imread-static.dir/src/_png.cpp.s

CMakeFiles/imread-static.dir/src/_png.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/_png.cpp.o.requires

CMakeFiles/imread-static.dir/src/_png.cpp.o.provides: CMakeFiles/imread-static.dir/src/_png.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/_png.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/_png.cpp.o.provides

CMakeFiles/imread-static.dir/src/_png.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/_png.cpp.o

CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o: ../src/_pvrtc.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o -c /Users/fish/Dropbox/libimread/src/_pvrtc.cpp

CMakeFiles/imread-static.dir/src/_pvrtc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/_pvrtc.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/_pvrtc.cpp > CMakeFiles/imread-static.dir/src/_pvrtc.cpp.i

CMakeFiles/imread-static.dir/src/_pvrtc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/_pvrtc.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/_pvrtc.cpp -o CMakeFiles/imread-static.dir/src/_pvrtc.cpp.s

CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o.requires

CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o.provides: CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o.provides

CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o

CMakeFiles/imread-static.dir/src/_tiff.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/_tiff.cpp.o: ../src/_tiff.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/_tiff.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/_tiff.cpp.o -c /Users/fish/Dropbox/libimread/src/_tiff.cpp

CMakeFiles/imread-static.dir/src/_tiff.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/_tiff.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/_tiff.cpp > CMakeFiles/imread-static.dir/src/_tiff.cpp.i

CMakeFiles/imread-static.dir/src/_tiff.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/_tiff.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/_tiff.cpp -o CMakeFiles/imread-static.dir/src/_tiff.cpp.s

CMakeFiles/imread-static.dir/src/_tiff.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/_tiff.cpp.o.requires

CMakeFiles/imread-static.dir/src/_tiff.cpp.o.provides: CMakeFiles/imread-static.dir/src/_tiff.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/_tiff.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/_tiff.cpp.o.provides

CMakeFiles/imread-static.dir/src/_tiff.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/_tiff.cpp.o

CMakeFiles/imread-static.dir/src/_webp.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/_webp.cpp.o: ../src/_webp.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/_webp.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/_webp.cpp.o -c /Users/fish/Dropbox/libimread/src/_webp.cpp

CMakeFiles/imread-static.dir/src/_webp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/_webp.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/_webp.cpp > CMakeFiles/imread-static.dir/src/_webp.cpp.i

CMakeFiles/imread-static.dir/src/_webp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/_webp.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/_webp.cpp -o CMakeFiles/imread-static.dir/src/_webp.cpp.s

CMakeFiles/imread-static.dir/src/_webp.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/_webp.cpp.o.requires

CMakeFiles/imread-static.dir/src/_webp.cpp.o.provides: CMakeFiles/imread-static.dir/src/_webp.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/_webp.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/_webp.cpp.o.provides

CMakeFiles/imread-static.dir/src/_webp.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/_webp.cpp.o

CMakeFiles/imread-static.dir/src/formats.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/formats.cpp.o: ../src/formats.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/formats.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/formats.cpp.o -c /Users/fish/Dropbox/libimread/src/formats.cpp

CMakeFiles/imread-static.dir/src/formats.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/formats.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/formats.cpp > CMakeFiles/imread-static.dir/src/formats.cpp.i

CMakeFiles/imread-static.dir/src/formats.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/formats.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/formats.cpp -o CMakeFiles/imread-static.dir/src/formats.cpp.s

CMakeFiles/imread-static.dir/src/formats.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/formats.cpp.o.requires

CMakeFiles/imread-static.dir/src/formats.cpp.o.provides: CMakeFiles/imread-static.dir/src/formats.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/formats.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/formats.cpp.o.provides

CMakeFiles/imread-static.dir/src/formats.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/formats.cpp.o

CMakeFiles/imread-static.dir/src/lzw.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/lzw.cpp.o: ../src/lzw.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/lzw.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/lzw.cpp.o -c /Users/fish/Dropbox/libimread/src/lzw.cpp

CMakeFiles/imread-static.dir/src/lzw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/lzw.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/lzw.cpp > CMakeFiles/imread-static.dir/src/lzw.cpp.i

CMakeFiles/imread-static.dir/src/lzw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/lzw.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/lzw.cpp -o CMakeFiles/imread-static.dir/src/lzw.cpp.s

CMakeFiles/imread-static.dir/src/lzw.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/lzw.cpp.o.requires

CMakeFiles/imread-static.dir/src/lzw.cpp.o.provides: CMakeFiles/imread-static.dir/src/lzw.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/lzw.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/lzw.cpp.o.provides

CMakeFiles/imread-static.dir/src/lzw.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/lzw.cpp.o

CMakeFiles/imread-static.dir/src/halide.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/halide.cpp.o: ../src/halide.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/halide.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/halide.cpp.o -c /Users/fish/Dropbox/libimread/src/halide.cpp

CMakeFiles/imread-static.dir/src/halide.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/halide.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/halide.cpp > CMakeFiles/imread-static.dir/src/halide.cpp.i

CMakeFiles/imread-static.dir/src/halide.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/halide.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/halide.cpp -o CMakeFiles/imread-static.dir/src/halide.cpp.s

CMakeFiles/imread-static.dir/src/halide.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/halide.cpp.o.requires

CMakeFiles/imread-static.dir/src/halide.cpp.o.provides: CMakeFiles/imread-static.dir/src/halide.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/halide.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/halide.cpp.o.provides

CMakeFiles/imread-static.dir/src/halide.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/halide.cpp.o

CMakeFiles/imread-static.dir/src/pvr.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/pvr.cpp.o: ../src/pvr.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/pvr.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/pvr.cpp.o -c /Users/fish/Dropbox/libimread/src/pvr.cpp

CMakeFiles/imread-static.dir/src/pvr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/pvr.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/pvr.cpp > CMakeFiles/imread-static.dir/src/pvr.cpp.i

CMakeFiles/imread-static.dir/src/pvr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/pvr.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/pvr.cpp -o CMakeFiles/imread-static.dir/src/pvr.cpp.s

CMakeFiles/imread-static.dir/src/pvr.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/pvr.cpp.o.requires

CMakeFiles/imread-static.dir/src/pvr.cpp.o.provides: CMakeFiles/imread-static.dir/src/pvr.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/pvr.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/pvr.cpp.o.provides

CMakeFiles/imread-static.dir/src/pvr.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/pvr.cpp.o

CMakeFiles/imread-static.dir/src/pvrtc.cpp.o: CMakeFiles/imread-static.dir/flags.make
CMakeFiles/imread-static.dir/src/pvrtc.cpp.o: ../src/pvrtc.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/fish/Dropbox/libimread/build2/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/imread-static.dir/src/pvrtc.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imread-static.dir/src/pvrtc.cpp.o -c /Users/fish/Dropbox/libimread/src/pvrtc.cpp

CMakeFiles/imread-static.dir/src/pvrtc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imread-static.dir/src/pvrtc.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/fish/Dropbox/libimread/src/pvrtc.cpp > CMakeFiles/imread-static.dir/src/pvrtc.cpp.i

CMakeFiles/imread-static.dir/src/pvrtc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imread-static.dir/src/pvrtc.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/fish/Dropbox/libimread/src/pvrtc.cpp -o CMakeFiles/imread-static.dir/src/pvrtc.cpp.s

CMakeFiles/imread-static.dir/src/pvrtc.cpp.o.requires:
.PHONY : CMakeFiles/imread-static.dir/src/pvrtc.cpp.o.requires

CMakeFiles/imread-static.dir/src/pvrtc.cpp.o.provides: CMakeFiles/imread-static.dir/src/pvrtc.cpp.o.requires
	$(MAKE) -f CMakeFiles/imread-static.dir/build.make CMakeFiles/imread-static.dir/src/pvrtc.cpp.o.provides.build
.PHONY : CMakeFiles/imread-static.dir/src/pvrtc.cpp.o.provides

CMakeFiles/imread-static.dir/src/pvrtc.cpp.o.provides.build: CMakeFiles/imread-static.dir/src/pvrtc.cpp.o

# Object files for target imread-static
imread__static_OBJECTS = \
"CMakeFiles/imread-static.dir/src/_bmp.cpp.o" \
"CMakeFiles/imread-static.dir/src/_jpeg.cpp.o" \
"CMakeFiles/imread-static.dir/src/_lsm.cpp.o" \
"CMakeFiles/imread-static.dir/src/_png.cpp.o" \
"CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o" \
"CMakeFiles/imread-static.dir/src/_tiff.cpp.o" \
"CMakeFiles/imread-static.dir/src/_webp.cpp.o" \
"CMakeFiles/imread-static.dir/src/formats.cpp.o" \
"CMakeFiles/imread-static.dir/src/lzw.cpp.o" \
"CMakeFiles/imread-static.dir/src/halide.cpp.o" \
"CMakeFiles/imread-static.dir/src/pvr.cpp.o" \
"CMakeFiles/imread-static.dir/src/pvrtc.cpp.o"

# External object files for target imread-static
imread__static_EXTERNAL_OBJECTS =

libimread-static.a: CMakeFiles/imread-static.dir/src/_bmp.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/_jpeg.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/_lsm.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/_png.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/_tiff.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/_webp.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/formats.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/lzw.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/halide.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/pvr.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/src/pvrtc.cpp.o
libimread-static.a: CMakeFiles/imread-static.dir/build.make
libimread-static.a: CMakeFiles/imread-static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libimread-static.a"
	$(CMAKE_COMMAND) -P CMakeFiles/imread-static.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imread-static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/imread-static.dir/build: libimread-static.a
.PHONY : CMakeFiles/imread-static.dir/build

CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/_bmp.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/_jpeg.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/_lsm.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/_png.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/_pvrtc.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/_tiff.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/_webp.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/formats.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/lzw.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/halide.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/pvr.cpp.o.requires
CMakeFiles/imread-static.dir/requires: CMakeFiles/imread-static.dir/src/pvrtc.cpp.o.requires
.PHONY : CMakeFiles/imread-static.dir/requires

CMakeFiles/imread-static.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/imread-static.dir/cmake_clean.cmake
.PHONY : CMakeFiles/imread-static.dir/clean

CMakeFiles/imread-static.dir/depend:
	cd /Users/fish/Dropbox/libimread/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/fish/Dropbox/libimread /Users/fish/Dropbox/libimread /Users/fish/Dropbox/libimread/build2 /Users/fish/Dropbox/libimread/build2 /Users/fish/Dropbox/libimread/build2/CMakeFiles/imread-static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/imread-static.dir/depend

