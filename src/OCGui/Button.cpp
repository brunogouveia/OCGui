#include <OCGui/Button.h>
#include <iostream>

namespace OCGui {

    Button::Button(std::string&& label):
        Widget(std::move(label))
    {
        // This is neede due to a bug on the button renderer.
        // Of course would be better to fix that, but whatever.
        m_label = m_label + " ";
    }

    Button::~Button() {}

    void Button::Draw()
    {
        // ImGui::Button(m_label.c_str());
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        ImGuiButtonFlags flags = 0;
        ImVec2 size_arg = ImVec2(0,0);
        ImGuiState& g = *((ImGuiState*)ImGui::GetInternalState());
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(m_label.c_str());
        const ImVec2 label_size = ImGui::CalcTextSize(m_label.c_str(), NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)
            pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
        ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ImGui::ItemSize(bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, &id))
            return;

        if (window->DC.ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
        bool hovered, held;
        ImGui::ButtonBehavior(bb, id, &hovered, &held, true, flags);

        // Render
        ImVec2 min = bb.Min;
        ImVec2 max = bb.Max;
        bndToolButton(ImGui::GetNVGcontext(), min.x, min.y,max.x - min.x,max.y - min.y, BND_CORNER_NONE, (hovered && held) ? BND_ACTIVE : hovered ? BND_HOVER : BND_DEFAULT, -1, m_label.c_str());

    }

} /* OCGui */
