#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Gui
{
	void init() noexcept;
	void fpsWindow() noexcept;
	void menuWindow() noexcept;
	void newFrame() noexcept;
	void render() noexcept;
	void shutdown() noexcept;
};
