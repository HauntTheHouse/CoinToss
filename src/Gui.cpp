#include "Gui.h"

#include <iostream>
#include <vector>
#include <string>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "System.h"

namespace Gui
{
    namespace
    {
        bool bIsMenuActive;
    }

    void init() noexcept
    {
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(System::getWindowParameters().mWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void fpsWindow() noexcept
    {
        ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
        ImGui::SetWindowPos({ 0, 0 });
        const auto& clearColor = System::getData().mClearColor;
        ImGui::TextColored({ (1.0f - clearColor.r), (1.0f - clearColor.g), (1.0f - clearColor.b), 1.0f }, "FPS: %.f\t", ImGui::GetIO().Framerate);
        ImGui::End();
    }

    void menuWindow() noexcept
    {
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetWindowPos(ImVec2{ System::getWindowParameters().mWidth - ImGui::GetWindowWidth(), 0.0f });

        ImGui::ColorEdit3("Clear color", &System::getData().mClearColor.x);

        auto& lightDir = System::getData().mLightDir;
        ImGui::SliderFloat3("Light direction", &lightDir.x, -1.0f, 1.0f);
        if (ImGui::IsItemActive())
        {
            lightDir = glm::normalize(lightDir);
            Shader::setActiveProgramId(System::getData().mModelsProgramId);
            Shader::setUniformVec3("uLightDir", lightDir);
        }

        ImTextureID texId = (ImTextureID)System::getData().mDepthToColorFramebuffer.getTexture();
        ImGui::Image(texId, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
        bIsMenuActive = ImGui::IsAnyItemActive();
        ImGui::End();
    }

    bool isMenuActive() noexcept
    {
        return bIsMenuActive;
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
