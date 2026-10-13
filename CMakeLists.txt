cmake_minimum_required(VERSION 3.20)

project(ReimanViz VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable GLFW documentation" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable GLFW examples" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

add_executable(ReimanViz src/main.cpp)
target_link_libraries(ReimanViz PRIVATE glfw OpenGL::GL)
target_compile_definitions(ReimanViz PRIVATE GLFW_INCLUDE_NONE)

if(APPLE)
    target_compile_definitions(ReimanViz PRIVATE GL_SILENCE_DEPRECATION)
endif()

if(MSVC)
    target_compile_options(ReimanViz PRIVATE /W4)
else()
    target_compile_options(ReimanViz PRIVATE -Wall -Wextra -Wpedantic)
endif()
