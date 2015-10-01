cmake_minimum_required (VERSION 2.8.11)
project (ImGuiExamples)

# add the -c and -Wall flags
add_definitions(
	-c
	-Wall
)

# We need a CMAKE_DIR with some code to find external dependencies
SET(ImGuiExamples_CMAKE_DIR "${ImGuiExamples_SOURCE_DIR}/../cmake")

# OpenGL
find_package(OpenGL REQUIRED)
# GLFW3
INCLUDE(${ImGuiExamples_CMAKE_DIR}/FindGLFW3.cmake)

# Lets LOAD app our sources!
file(GLOB_RECURSE SRCS1
    ${ImGuiExamples_SOURCE_DIR}/example.cpp
	${ImGuiExamples_SOURCE_DIR}/../src/Blendish/*.c
	${ImGuiExamples_SOURCE_DIR}/../src/Blendish/blendish.c
	${ImGuiExamples_SOURCE_DIR}/../src/ImGui/*.cpp
	${ImGuiExamples_SOURCE_DIR}/../src/ImGui/*.c
	${ImGuiExamples_SOURCE_DIR}/../src/OCGui/*.cpp
    ${ImGuiExamples_SOURCE_DIR}/../libs/gl3w/GL/*.c
)

# Lets LOAD app our sources!
file(GLOB_RECURSE SRCS2
    ${ImGuiExamples_SOURCE_DIR}/example2.cpp
	${ImGuiExamples_SOURCE_DIR}/../src/Blendish/*.c
	${ImGuiExamples_SOURCE_DIR}/../src/Blendish/blendish.c
	${ImGuiExamples_SOURCE_DIR}/../src/ImGui/*.cpp
	${ImGuiExamples_SOURCE_DIR}/../src/ImGui/*.c
	${ImGuiExamples_SOURCE_DIR}/../src/OCGui/*.cpp
    ${ImGuiExamples_SOURCE_DIR}/../libs/gl3w/GL/*.c
)

# Lets LOAD app our headers!
file(GLOB_RECURSE HDRS ${ImGuiExamples_SOURCE_DIR}/../include/*.h ${GLFW3_INCLUDE_DIRS}/GLFW/*.h)

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
    ${ImGuiExamples_SOURCE_DIR}/../include
    ${GLFW3_INCLUDE_PATH}
    ${ImGuiExamples_SOURCE_DIR}/../libs/gl3w
)

add_executable (example ${HDRS} ${SRCS1})
add_executable (example2 ${HDRS} ${SRCS2})

target_link_libraries(example ${GLFW3_LIBRARIES} "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
target_link_libraries(example2 ${GLFW3_LIBRARIES} "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
