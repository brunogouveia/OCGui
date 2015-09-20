#ifndef WIDGET_H__
#define WIDGET_H__

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw_gl3.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace OCGui
{
    class Widget
    {
    public:
        Widget(std::string&& label) : m_label(label) {};
        virtual ~Widget() {};

        virtual void Draw() = 0;

        inline void AddChild(Widget* child) { m_children.push_back(child); }

    protected:
        std::string          m_label;
        std::vector<Widget*> m_children;
    };
}

#endif
