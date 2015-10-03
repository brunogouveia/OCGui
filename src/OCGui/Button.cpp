#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/Button.h>
#include <iostream>

namespace OCGui {

    Button::Button(std::string&& label):
        Widget(std::move(label)),
        m_pressed(false),
        m_hovered(false),
        m_held(false)
    {
        // This is neede due to a bug on the button renderer.
        // Of course would be better to fix that, but whatever.
        m_label = m_label + " ";
    }

    Button::~Button() {}

    void Button::Draw(Vec2&& position, Vec2&& size)
    {
        ImGuiState& g = *((ImGuiState*)ImGui::GetInternalState());
        const ImGuiStyle& style = g.Style;
        
        // Render
        ImVec2 pos = m_position + style.FramePadding * 2;
        ImVec2 correctSize = m_size - style.FramePadding * 4;
        bndToolButton(ImGui::GetNVGcontext(), pos.x, pos.y, correctSize.x, correctSize.y, BND_CORNER_NONE, (m_hovered && m_held) ? BND_ACTIVE : m_hovered ? BND_HOVER : BND_DEFAULT, -1, m_label.c_str());
        
    }
    
    bool Button::HandleEvents(Vec2&& position, Vec2&& size)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        
        ImGuiButtonFlags flags = 0;
        ImGuiState& g = *((ImGuiState*)ImGui::GetInternalState());
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(m_label.c_str());
        const ImVec2 label_size = ImGui::CalcTextSize(m_label.c_str(), NULL, true);
        
        ImVec2 pos = position + style.FramePadding * 2;
        ImVec2 correctSize = (size - style.FramePadding * 4).NonNegative();
        
        if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)
            pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
        ImVec2 actualSize = ImGui::CalcItemSize(correctSize, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
        
        const ImRect bb(pos, pos + actualSize);
        ImGui::ItemSize(bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, &id))
            return false;
        
        if (window->DC.ButtonRepeat) {
            flags |= ImGuiButtonFlags_Repeat;
        }
        m_pressed = ImGui::ButtonBehavior(bb, id, &m_hovered, &m_held, true, flags);
        
        // Update position and size
        m_position = position;
        m_size     = bb.Max - bb.Min + style.FramePadding * 4;
        
        return Widget::HandleEvents(std::move(position), std::move(size));
    }

} /* OCGui */

#endif
