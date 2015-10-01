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

    class Widget
    {
    public:
        Widget(std::string&& label) : m_label(label) {}
        virtual ~Widget() {}

        virtual void Draw() = 0;

        virtual void Active();

        virtual void OnMouseClick(const MouseEvent& event) { std::cout << "MouseClick on: " << m_label << std::endl; }
        virtual void OnMouseRelease(const MouseEvent& event) { std::cout << "MouseRelease on: " << m_label << std::endl; }
        virtual void OnMouseHover(const MouseEvent& event) { std::cout << "MouseHover on: " << m_label << std::endl; }
        virtual void OnMouseDown(const MouseEvent& event) { std::cout << "MouseDown on: " << m_label << std::endl; }
        virtual void OnMouseUp(const MouseEvent& event) { std::cout << "MouseUp on: " << m_label << std::endl; }
        virtual void OnDrag(const DragEvent& event) { std::cout << "OnDrag on: " << m_label << std::endl; std::cout << ImGui::GetMouseDragDelta().x << " " << ImGui::GetMouseDragDelta().x << std::endl; }
        virtual void OnDrop(const DropEvent& event) { std::cout << "OnDrop on: " << m_label << " dropped: " << event.widgetDropped->m_label << std::endl; }

        inline void AddChild(Widget* child) { m_children.push_back(child); }

    protected:
        std::string          m_label;
        std::vector<Widget*> m_children;
    };
}

#endif
