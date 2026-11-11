cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)  # disable compiler-specific extensions

set(n matmul-assignment)
project(${n})

if(CMAKE_SYSTEM_NAME STREQUAL "Prospero")
  include(deps/ps5-bits/ps5-libs.cmake)
endif()

# ps5-libc-heap.cpp gives the PlayStation 5 a heap worth having.  It used to be
# an #ifdef __PROSPERO__ block at the top of the program; it is here instead
# because it is nothing to do with multiplying matrices, and because every
# project in the module now shares the one copy.  The two symbols it defines
# are read by the SDK before main() runs, so the file has to be linked in
# rather than included.  Off the PS5 it compiles to nothing.
#
# source_group files it under "PlayStation" in Visual Studio, so that "Source
# Files" holds the files the assignment is actually about.
set(ps5-heap deps/ps5-bits/ps5-libc-heap.cpp)
source_group("PlayStation" FILES ${ps5-heap})

add_executable(${n} src/${n}.cpp src/stats.cpp src/stats.hpp ${ps5-heap})

# sdl3-print.hpp stands in for C++23's <print>, by way of {fmt}.  Both are
# header-only, so there is nothing to build or link - only these two
# directories to look in.  MSVC needs /utf-8 for {fmt}'s own sources.
target_include_directories(${n} PRIVATE deps/sdl3-uws/include)
target_include_directories(${n} PRIVATE deps/fmt-12.2.0/include)
target_compile_options(${n} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

# The PlayStation 5's system stub libraries, this one among them: the program
# uses pthreads, which are ScePosix there.
if(CMAKE_SYSTEM_NAME STREQUAL "Prospero")
  ps5_link_system_libs(${n})
endif()

#target_include_directories(${name} PRIVATE $ENV{SCE_PROSPERO_SAMPLE_DIR}/sample_code/common/include)

#target_link_directories(${name} PRIVATE $ENV{SCE_PROSPERO_SAMPLE_DIR}/sample_code/common/source/sampleutil/Prospero_Debug) # or Prospero_Release
#target_link_libraries(${name} SceSampleUtil)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${n})
