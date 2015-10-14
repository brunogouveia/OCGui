#ifndef WIDGET_H__
#define WIDGET_H__

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui_impl_glfw_gl3.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

#include <OCGui/Events.h>

namespace OCGui
{

    typedef ImVec2 Vec2;
    typedef ImVec2 Size;
    typedef ImRect Rect;
    typedef std::string String;
    
    void Init(const String& title, const Size& size);
    void ShutDown();
    void BeginFrame();
    void EndFrame();
    
    void HandlGLFWEvents();
    bool IsCloseRequested();
    void SwapBuffers();

    Size GetWindowSize();
    Size GetFramebufferSize();

}

#endif
