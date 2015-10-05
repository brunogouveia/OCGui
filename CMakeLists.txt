cmake_minimum_required (VERSION 2.8.11)
project (OCGui)

# add the -c and -Wall flags
add_definitions(
	-c
	-Wall
)

# We need a CMAKE_DIR with some code to find external dependencies
SET(OCGui_CMAKE_DIR "${OCGui_SOURCE_DIR}/cmake")

# OpenGL
find_package(OpenGL REQUIRED)
# GLFW3
INCLUDE(${OCGui_CMAKE_DIR}/FindGLFW3.cmake)

# Lets LOAD app our sources!
file(GLOB_RECURSE SRCS
	${OCGui_SOURCE_DIR}/src/Blendish/*.c
	${OCGui_SOURCE_DIR}/src/Blendish/blendish.c
	${OCGui_SOURCE_DIR}/src/ImGui/*.cpp
	${OCGui_SOURCE_DIR}/src/ImGui/*.c
	${OCGui_SOURCE_DIR}/src/OCGui/*.cpp
    ${OCGui_SOURCE_DIR}/libs/gl3w/GL/*.c
)

# Lets LOAD app our headers!
file(GLOB_RECURSE HDRS ${OCGui_SOURCE_DIR}/../include/*.h ${GLFW3_INCLUDE_DIRS}/GLFW/*.h)

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
    ${OCGui_SOURCE_DIR}/include
    ${GLFW3_INCLUDE_PATH}
    ${OCGui_SOURCE_DIR}/libs/gl3w
)

add_library (OCGui ${HDRS} ${SRCS})

target_include_directories(OCGui
	PUBLIC
    ${OCGui_SOURCE_DIR}/include
    ${GLFW3_INCLUDE_PATH}
    ${OCGui_SOURCE_DIR}/libs/gl3w
)

target_link_libraries(OCGui ${GLFW3_LIBRARIES} "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
