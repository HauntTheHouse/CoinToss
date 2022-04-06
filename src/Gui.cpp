#include "Gui.h"
#include "Singleton.h"
#include "WindowParameters.h"
#include "Data.h"

namespace Gui
{
    void init() noexcept
    {
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(Singleton<WindowParameters>::getInstance().mWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void fpsWindow() noexcept
    {
        ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
        ImGui::SetWindowPos({ 0, 0 });
        ImGui::TextColored({ 0.0f, 0.0f, 0.0f, 1.0f }, "FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
    }

    void menuWindow() noexcept
    {
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::ColorEdit3("Clear color", &Singleton<Data>::getInstance().mClearColor.x);
        ImGui::SetWindowPos(ImVec2{ Singleton<WindowParameters>::getInstance().mWidth - ImGui::GetWindowWidth(), 0.0f });
        ImGui::End();
    }

    void newFrame() noexcept
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void render() noexcept
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void shutdown() noexcept
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
