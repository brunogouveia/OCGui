#include <OCGui/Window.h>

namespace OCGui
{
    static void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error %d: %s\n", error, description);
    }

    Window::Window(std::string&& label, int width, int height) :
        Widget(std::move(label)),
        m_width(width),
        m_height(height)
    {
        // Setup window
        glfwSetErrorCallback(error_callback);
        if (!glfwInit())
            exit(1);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
        m_window = glfwCreateWindow(m_width, m_height, m_label.c_str(), NULL, NULL);
        glfwMakeContextCurrent(m_window);
        gl3wInit();

        // Setup ImGui binding
        ImGui_ImplGlfwGL3_Init(m_window, true);

        // Load Fonts
        // (see extra_fonts/README.txt for more details)
        ImGuiIO& io = ImGui::GetIO();
        //io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("../extra_fonts/Cousine-Regular.ttf", 15.0f);
        io.Fonts->AddFontFromFileTTF("../extra_fonts/DroidSans.ttf", 16.0f);

    }

    Window::~Window() {

    }

    void Window::Draw()
    {
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::SetWindowPos(ImVec2(0,0));
        ImGui::SetWindowSize(ImVec2(m_width, m_height));

        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(114, 114, 154, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        for (std::vector<Widget*>::iterator child = m_children.begin(); child != m_children.end(); child++)
        {
            (*child)->Draw();
        }

        ImGui::Render();

    }

}
