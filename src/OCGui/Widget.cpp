#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/Widget.h>
#include <map>

namespace OCGui
{

    static Widget* m_childBeingDragged = NULL;
    
    static GLFWwindow* g_window = nullptr;
    static Vec2        g_windowSize;
    
    static void errorCallback(int error, const char* description)
    {
        fprintf(stderr, "Error %d: %s\n", error, description);
    }
    
    void Init(const String& title, const Size& size)
    {
        // Set size
        g_windowSize = size;
        
        // Setup window
        glfwSetErrorCallback(errorCallback);
        if (!glfwInit())
            exit(1);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DEPTH_BITS, 32);
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        g_window = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);
        glfwMakeContextCurrent(g_window);
        
        glfwSwapInterval(1);
        
        gl3wInit();
        
        // Setup ImGui binding
        ImGui_ImplGlfwGL3_Init(g_window, true);
        
        // Load Fonts
        // (see extra_fonts/README.txt for more details)
        ImGuiIO& io = ImGui::GetIO();
        //io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("../extra_fonts/Cousine-Regular.ttf", 15.0f);
        io.Fonts->AddFontFromFileTTF("../FiraMono-Regular.ttf", 19.0f);
        
        ImGuiState& g = *GImGui;
        ImGuiStyle& style = g.Style;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(33.0/255.0,37.0/255.0,42.0/255.0,1);
        style.Colors[ImGuiCol_FrameBg]  = ImVec4(40.0/255.0,44.0/255.0,51.0/255.0,1);
        style.Colors[ImGuiCol_Border]   = ImVec4(23.0/255.0,27.0/255.0,32.0/255.0,1);
        style.Colors[ImGuiCol_Button]   = ImVec4(56.0/255.0,61.0/255.0,71.0/255.0,1);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(61.0/255.0,65.0/255.0,78.0/255.0,1);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(44.0/255.0,49.0/255.0,58.0/255.0,1);
        style.Colors[ImGuiCol_TextCursor] = ImVec4(81.0/255.0,139.0/255.0,1.0,1);
        style.FrameRounding = 4;
        style.Alpha = 1.0;
    }
    
    void ShutDown()
    {
        // Cleanup
        ImGui_ImplGlfwGL3_Shutdown();
        glfwTerminate();
    }
    
    void BeginFrame()
    {
        int display_w, display_h;
        glfwGetFramebufferSize(g_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        
        int w, h;
        glfwGetWindowSize(g_window, &w, &h);
        
        // Create new ImGui frame
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::SetWindowPos(ImVec2(0,0));
        ImGui::SetWindowSize(Size(w, h));
        ImGui::GetCurrentWindow()->Flags |= ImGuiWindowFlags_ShowBorders;
    }
    
    void EndFrame()
    {
        ImVec4 bgColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
        glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
    }
    
    void HandlGLFWEvents()
    {
        glfwPollEvents();
    }
    
    bool IsCloseRequested()
    {
        return glfwWindowShouldClose(g_window);
    }

    void SwapBuffers()
    {
        glfwSwapBuffers(g_window);
    }
    
    Size GetWindowSize()
    {
        int w, h;
        glfwGetWindowSize(g_window, &w, &h);
        return Size(w, h);
    }
    
    Size GetFramebufferSize()
    {
        int w, h;
        glfwGetFramebufferSize(g_window, &w, &h);
        return Size(w, h);
    }

    // Multi-thread
    std::map<String,GLFWwindow*> g_contextMap;

    GLFWwindow* CreateContext(String name)
    {
        GLFWwindow *newWindow = glfwCreateWindow(1, 1, name.c_str(), nullptr, g_window);
        g_contextMap.insert(std::pair<String, GLFWwindow*>(name, newWindow));
        return newWindow;
    }

    GLFWwindow* GetContext(String name)
    {
        // This return NULL, when the value is not found.
        return g_contextMap[name];
    }

    void MakeContextCurrent(String name)
    {
        glfwMakeContextCurrent(g_contextMap[name]);
    }

    void DestroyContext(String name)
    {
        glfwDestroyWindow(g_contextMap[name]);
    }

} /* OCGui */

#endif
