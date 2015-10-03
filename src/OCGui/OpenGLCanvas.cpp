#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/OpenGLCanvas.h>

namespace OCGui
{
    OpenGLCanvas::OpenGLCanvas(std::string&& label, int width, int height):
        Widget(std::move(label)),
        m_drawCallback(NULL),
        m_width(width),
        m_height(height)
    {

    }

    OpenGLCanvas::~OpenGLCanvas() {}

    void OpenGLCanvas::Draw(Vec2&& position, Vec2&& size)
    {
        if (m_content) {
            m_content->Draw(std::move(position), std::move(size));
        }
        
        if (GetDrawCallback())
        {
            GetDrawCallback()(std::move(m_position), m_position + m_size);
        }
    }
    
    bool OpenGLCanvas::HandleEvents(Vec2&& position, Vec2&& size)
    {
        if (m_content && m_content->HandleEvents(std::move(position), std::move(size)))
        {
            return true;
        }
        
        using namespace ImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;
        
        ImGuiState& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(m_label.c_str());
        
        Vec2 actualSize = Vec2(size.x ? size.x : m_width, size.y ? size.y : m_height);
        
        const ImRect bb(position, position + actualSize);
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, &id))
            return false;
        
        // Behavior
        const bool hovered = IsHovered(bb, id, true);
        if (hovered)
        {
            g.HoveredId = id;
            if ((!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
            {
                if (g.IO.MouseClicked[0])
                {
                    SetActiveID(id, window);
                    FocusWindow(window);
                }
            }
        } else if (g.ActiveId == id) {
            if (g.IO.MouseClicked[0])
            {
                SetActiveID(0);
            }
        }
        
        bool held = false;
        if (g.ActiveId == id)
        {
            g.ActiveIdIsFocusedOnly = !g.IO.MouseDown[0];
            if (g.IO.MouseDown[0])
            {
                held = true;
            }
        }
        
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            std::cout << "Canvas clicked" << std::endl;
        }
        
        // Update position and size
        m_position = position;
        m_size     = actualSize;

        return false;
    }

} /* OCGui */

#endif
