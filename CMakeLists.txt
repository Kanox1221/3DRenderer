cmake_minimum_required(VERSION 3.16)
project(Renderer VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)

# Assimp Builds
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/assimp)

find_package(OpenGL REQUIRED)
# find_package(glfw REQUIRED)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

add_executable(Renderer
    # src/main.cpp
    external/glad/glad.c
    # src/readShaderFile.cpp
    ${SOURCE_FILES}
)

target_include_directories(Renderer PRIVATE external external/glfw/include)
target_include_directories(Renderer PRIVATE external external/glm)
target_link_libraries(Renderer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/lib/glfw3.lib")
target_link_libraries(Renderer PRIVATE user32)
target_link_libraries(Renderer PRIVATE gdi32)
target_link_libraries(Renderer PRIVATE shell32)
target_link_libraries(Renderer PRIVATE opengl32)
target_link_libraries(Renderer PRIVATE assimp::assimp)

set_property(TARGET Renderer PROPERTY MSVC_RUNTIME_LIBRARY MultiThreadedDLL)

target_compile_features(Renderer PRIVATE cxx_std_17)
target_compile_definitions(Renderer PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
