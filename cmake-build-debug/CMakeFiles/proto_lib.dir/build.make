# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\w2552\CLionProjects\GateServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\w2552\CLionProjects\GateServer\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/proto_lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/proto_lib.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/proto_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/proto_lib.dir/flags.make

CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.obj: CMakeFiles/proto_lib.dir/flags.make
CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.obj: CMakeFiles/proto_lib.dir/includes_CXX.rsp
CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.obj: C:/Users/w2552/CLionProjects/GateServer/Sources/protos/verify.pb.cc
CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.obj: CMakeFiles/proto_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\w2552\CLionProjects\GateServer\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.obj"
	D:\MSYS2\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.obj -MF CMakeFiles\proto_lib.dir\Sources\protos\verify.pb.cc.obj.d -o CMakeFiles\proto_lib.dir\Sources\protos\verify.pb.cc.obj -c C:\Users\w2552\CLionProjects\GateServer\Sources\protos\verify.pb.cc

CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.i"
	D:\MSYS2\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\w2552\CLionProjects\GateServer\Sources\protos\verify.pb.cc > CMakeFiles\proto_lib.dir\Sources\protos\verify.pb.cc.i

CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.s"
	D:\MSYS2\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\w2552\CLionProjects\GateServer\Sources\protos\verify.pb.cc -o CMakeFiles\proto_lib.dir\Sources\protos\verify.pb.cc.s

CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.obj: CMakeFiles/proto_lib.dir/flags.make
CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.obj: CMakeFiles/proto_lib.dir/includes_CXX.rsp
CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.obj: C:/Users/w2552/CLionProjects/GateServer/Sources/protos/verify.grpc.pb.cc
CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.obj: CMakeFiles/proto_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\w2552\CLionProjects\GateServer\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.obj"
	D:\MSYS2\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.obj -MF CMakeFiles\proto_lib.dir\Sources\protos\verify.grpc.pb.cc.obj.d -o CMakeFiles\proto_lib.dir\Sources\protos\verify.grpc.pb.cc.obj -c C:\Users\w2552\CLionProjects\GateServer\Sources\protos\verify.grpc.pb.cc

CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.i"
	D:\MSYS2\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\w2552\CLionProjects\GateServer\Sources\protos\verify.grpc.pb.cc > CMakeFiles\proto_lib.dir\Sources\protos\verify.grpc.pb.cc.i

CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.s"
	D:\MSYS2\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\w2552\CLionProjects\GateServer\Sources\protos\verify.grpc.pb.cc -o CMakeFiles\proto_lib.dir\Sources\protos\verify.grpc.pb.cc.s

# Object files for target proto_lib
proto_lib_OBJECTS = \
"CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.obj" \
"CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.obj"

# External object files for target proto_lib
proto_lib_EXTERNAL_OBJECTS =

libproto_lib.a: CMakeFiles/proto_lib.dir/Sources/protos/verify.pb.cc.obj
libproto_lib.a: CMakeFiles/proto_lib.dir/Sources/protos/verify.grpc.pb.cc.obj
libproto_lib.a: CMakeFiles/proto_lib.dir/build.make
libproto_lib.a: CMakeFiles/proto_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\w2552\CLionProjects\GateServer\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libproto_lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles\proto_lib.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\proto_lib.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/proto_lib.dir/build: libproto_lib.a
.PHONY : CMakeFiles/proto_lib.dir/build

CMakeFiles/proto_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\proto_lib.dir\cmake_clean.cmake
.PHONY : CMakeFiles/proto_lib.dir/clean

CMakeFiles/proto_lib.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\w2552\CLionProjects\GateServer C:\Users\w2552\CLionProjects\GateServer C:\Users\w2552\CLionProjects\GateServer\cmake-build-debug C:\Users\w2552\CLionProjects\GateServer\cmake-build-debug C:\Users\w2552\CLionProjects\GateServer\cmake-build-debug\CMakeFiles\proto_lib.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/proto_lib.dir/depend

