#ifdef OCGUI_IMPLEMENTATION__

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
        m_height(height),
        m_childBeingDragged(NULL)
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

        // Initialize NVG context
        ImGui::SetNVGcontext(nvgCreateGL3(NVG_ANTIALIAS));
        bndSetFont(nvgCreateFont(ImGui::GetNVGcontext(), "system", "../DejaVuSans.ttf"));
        bndSetIconImage(nvgCreateImage(ImGui::GetNVGcontext(), "../blender_icons16.png", 0));

    }

    Window::~Window() {

    }

    void Window::Draw()
    {
        // Clear background
        nvgBeginFrame(ImGui::GetNVGcontext(), m_width, m_height, 2);
        bndBackground(ImGui::GetNVGcontext(), 0, 0, m_width, m_height);
        nvgEndFrame(ImGui::GetNVGcontext());
        
        // Create new ImGui frame
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::SetWindowPos(ImVec2(0,0));
        ImGui::SetWindowSize(ImVec2(m_width, m_height));

        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        
        // Begin nvg frame
        nvgBeginFrame(ImGui::GetNVGcontext(), m_width, m_height, 2);

        for (std::vector<Widget*>::iterator child = m_children.begin(); child != m_children.end(); child++)
        {
            (*child)->Draw();

            if (ImGui::IsItemHovered()) {
                if (ImGui::IsMouseClicked(0)) {
                    (*child)->OnMouseClick(MouseEvent());
                }
                else if (ImGui::IsMouseDown(0)) {
                    (*child)->OnMouseDown(MouseEvent());
                }
                else if (ImGui::IsMouseReleased(0))
                {
                    (*child)->OnMouseRelease(MouseEvent());
                }

                // Check for drop event
                if(m_childBeingDragged)
                {
                    if (m_childBeingDragged != (*child))
                        (*child)->OnDrop(DropEvent(m_childBeingDragged));
                    m_childBeingDragged = NULL;
                }

            }

            if (ImGui::IsItemActive() && ImGui::IsMouseDragging()) {
                (*child)->OnDrag(DragEvent(ImGui::GetMouseDragDelta()));
                m_childBeingDragged = (*child);
            }
        }

        ImGui::Render();

        nvgEndFrame(ImGui::GetNVGcontext());

    }

    Vec2 Window::GetFramebufferSize()
    {
        int x, y;
        glfwGetFramebufferSize(m_window, &x, &y);
        return Vec2(x, y);
    }

    Vec2 Window::GetWindowSize()
    {
        int x, y;
        glfwGetWindowSize(m_window, &x, &y);
        return Vec2(x, y);
    }

}

#endif
