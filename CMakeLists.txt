cmake_minimum_required(VERSION 3.16)
project(3d_renderer VERSION 1.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
# find_package(glfw3 REQUIRED)

add_executable(Renderer
    src/main.cpp
    external/glad/glad.c
)

target_include_directories(Renderer PRIVATE external external/glfw/include)
target_link_libraries(Renderer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/lib/glfw3.lib")
target_link_libraries(Renderer PRIVATE user32)
target_link_libraries(Renderer PRIVATE gdi32)
target_link_libraries(Renderer PRIVATE shell32)
target_link_libraries(Renderer PRIVATE opengl32)

set_property(TARGET Renderer PROPERTY MSVC_RUNTIME_LIBRARY MultiThreadedDLL)

target_compile_features(Renderer PRIVATE cxx_std_17)
