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
        Widget(std::string&& label):
            m_label(label),
            m_position(0,0),
            m_size(0,0),
            m_relativeSize(0,0)
        {}
        virtual ~Widget() {}

        virtual void Draw(Vec2&& position, Vec2&& size) = 0;
        virtual bool HandleEvents(Vec2&& position, Vec2&& size) = 0;

        virtual void OnMouseClick(const MouseEvent& event) { std::cout << "MouseClick on: " << m_label << std::endl; }
        virtual void OnMouseRelease(const MouseEvent& event) { std::cout << "MouseRelease on: " << m_label << std::endl; }
        virtual void OnMouseHover(const MouseEvent& event) { std::cout << "MouseHover on: " << m_label << std::endl; }
        virtual void OnMouseDown(const MouseEvent& event) { std::cout << "MouseDown on: " << m_label << std::endl; }
        virtual void OnMouseUp(const MouseEvent& event) { std::cout << "MouseUp on: " << m_label << std::endl; }
        virtual void OnDrag(const DragEvent& event) { std::cout << "OnDrag on: " << m_label << std::endl; std::cout << ImGui::GetMouseDragDelta().x << " " << ImGui::GetMouseDragDelta().x << std::endl; }
        virtual void OnDrop(const DropEvent& event) { std::cout << "OnDrop on: " << m_label << " dropped: " << event.widgetDropped->m_label << std::endl; }
        
        void SetRelativeSize(Vec2&& relativeSize) { m_relativeSize = relativeSize; }
        Vec2 GetRelativeSize() const { return m_relativeSize; }
        Vec2 GetPosition()     const { return m_position; }
        Vec2 GetSize()         const { return m_size; }
        const std::string GetLabel() const { return m_label; }

    protected:
        std::string m_label;
        Vec2        m_position;
        Vec2        m_size;
        Vec2        m_relativeSize;
    };
}

#endif
