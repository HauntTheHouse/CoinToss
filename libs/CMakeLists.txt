cmake_minimum_required(VERSION 3.0.0)
project (libs)

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(bullet3)
add_subdirectory(assimp)

add_library(glad
	glad/include/glad/glad.h
	glad/include/KHR/khrplatform.h
	glad/src/glad.c
)
target_include_directories(glad PRIVATE glad/include)

add_library(imgui
	imgui/imgui.h
	imgui/imgui.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PRIVATE imgui glfw/include)
