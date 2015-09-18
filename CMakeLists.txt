cmake_minimum_required (VERSION 2.8.11)
project (ImGui)

# add the -c and -Wall flags
add_definitions(
	-c
	-Wall
)

# Lets LOAD app our sources!
file(GLOB_RECURSE SRCS
    ${ImGui_SOURCE_DIR}/example.cpp
	${ImGui_SOURCE_DIR}/src/*.cpp
	${ImGui_SOURCE_DIR}/src/*.c
    ${ImGui_SOURCE_DIR}/libs/gl3w/GL/*.c
)

# Lets LOAD app our headers!
file(GLOB_RECURSE HDRS ${ImGui_SOURCE_DIR}/include/*.h)

# We need a CMAKE_DIR with some code to find external dependencies
SET(ImGui_CMAKE_DIR "${ImGui_SOURCE_DIR}/cmake")

# OpenGL
find_package(OpenGL REQUIRED)
# GLFW3
INCLUDE(${ImGui_CMAKE_DIR}/FindGLFW3.cmake)

# We are using C++14
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX14)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

include_directories(
    ${ImGui_SOURCE_DIR}/include
    ${GLFW3_INCLUDE_DIRS}
    ${ImGui_SOURCE_DIR}/libs/gl3w
)

add_executable (example ${HDRS} ${SRCS})

target_link_libraries(example ${GLFW3_LIBRARIES} "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
