#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class WindowParameters
{
public:
    WindowParameters() = default;
    ~WindowParameters()
    {
        glfwDestroyWindow(mWindow);
    }

    void init(int aWidth, int aHeigth, const std::string& aName)
    {
        mWidth = aWidth;
        mHeight = aHeigth;
        mName = aName;
        mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr);
    }

    GLFWwindow* mWindow{ nullptr };
    int mWidth{ 0 };
    int mHeight{ 0 };
    std::string mName;
};

template<typename T>
class Singleton
{
public:
    Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& getInstance()
    {
        static T instance;
        return instance;
    }
};

inline void windowInit() noexcept
{
    if (!glfwInit())
    {
        std::cout << "Failed to init GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    Singleton<WindowParameters>::getInstance().init(800, 600, "MoneyHeist");

    if (!Singleton<WindowParameters>::getInstance().mWindow)
    {
        std::cout << "Failed to create GLFWwindow" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(Singleton<WindowParameters>::getInstance().mWindow);
}

inline void gladInit() noexcept
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        return;
    }
}

inline void imguiInit() noexcept
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(Singleton<WindowParameters>::getInstance().mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

inline void showFps() noexcept
{
    constexpr auto guiWidth = ImVec2{ 90.0f, 30.0f };

    ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos({ Singleton<WindowParameters>::getInstance().mWidth - guiWidth.x, 0});
    ImGui::SetWindowSize(guiWidth);
    ImGui::TextColored({ 0.0f, 0.0f, 0.0f, 1.0f }, "FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}

inline void imguiNewFrame() noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

inline void render() noexcept
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

inline void releaseMemory() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

int main()
{
    windowInit();
    gladInit();
    imguiInit();

    ImVec4 clearColor = ImVec4(0.73f, 0.73f, 0.73f, 1.0f);

    glViewport(0, 0, Singleton<WindowParameters>::getInstance().mWidth, Singleton<WindowParameters>::getInstance().mHeight);

    while (!glfwWindowShouldClose(Singleton<WindowParameters>::getInstance().mWindow))
    {
        imguiNewFrame();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

        glViewport(0, 0, Singleton<WindowParameters>::getInstance().mWidth, Singleton<WindowParameters>::getInstance().mHeight);

        showFps();

        render();

        glfwPollEvents();
        glfwSwapBuffers(Singleton<WindowParameters>::getInstance().mWindow);
    }

    releaseMemory();
}
