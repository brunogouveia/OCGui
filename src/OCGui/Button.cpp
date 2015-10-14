#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/Button.h>
#include <iostream>

namespace OCGui
{
    static bool ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, bool allow_key_modifiers, ImGuiButtonFlags flags)
    {
        ImGuiState& g = *GImGui;
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        
        if (flags & ImGuiButtonFlags_Disabled)
        {
            if (out_hovered) *out_hovered = false;
            if (out_held) *out_held = false;
            if (g.ActiveId == id) ImGui::SetActiveID(0);
            return false;
        }
        
        bool pressed = false;
        const bool hovered = ImGui::IsHovered(bb, id, (flags & ImGuiButtonFlags_FlattenChilds) != 0);
        if (hovered)
        {
            g.HoveredId = id;
            if (allow_key_modifiers || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
            {
                if (g.IO.MouseClicked[0])
                {
                    if (flags & ImGuiButtonFlags_PressedOnClick)
                    {
                        pressed = true;
                        ImGui::SetActiveID(0);
                    }
                    else
                    {
                        ImGui::SetActiveID(id, window);
                    }
                    ImGui::FocusWindow(window);
                }
                else if (g.IO.MouseReleased[0] && (flags & ImGuiButtonFlags_PressedOnRelease))
                {
                    pressed = true;
                    ImGui::SetActiveID(0);
                }
                else if ((flags & ImGuiButtonFlags_Repeat) && g.ActiveId == id && ImGui::IsMouseClicked(0, true))
                {
                    pressed = true;
                }
            }
        }
        
        bool held = false;
        if (g.ActiveId == id)
        {
            if (g.IO.MouseDown[0])
            {
                held = true;
            }
            else
            {
                if (hovered)
                    pressed = true;
                ImGui::SetActiveID(0);
            }
        }
        
        if (out_hovered) *out_hovered = hovered;
        if (out_held) *out_held = held;
        
        return pressed;
    }
    
    static bool ButtonEx(const String& label, const Rect& rect, bool border, ImGuiButtonFlags flags)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;
        
        ImGuiState& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label.c_str());
        const ImVec2 label_size = ImGui::CalcTextSize(label.c_str(), NULL, true);
        
        ImVec2 pos = rect.GetPos();
        ImVec2 size = ImGui::CalcItemSize(rect.GetSize(), label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
        
        const ImRect bb(pos, pos + size);
        ImGui::ItemSize(bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, &id))
            return false;
        
        if (window->DC.ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, true, flags);
        
        // Render
        const ImU32 col = window->Color((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        ImGui::RenderFrame(bb.Min, bb.Max, col, border, style.FrameRounding);
        ImGui::RenderTextClipped(bb.Min, bb.Max, label.c_str(), NULL, &label_size, ImGuiAlign_Center | ImGuiAlign_VCenter);
        
        // Automatically close popups
        //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
        //    ImGui::CloseCurrentPopup();
        
        return pressed;
    }
    
    bool Button(const String& label, const Rect& area, bool border)
    {
        return ButtonEx(label, area, border, 0);
    }
    
    namespace Layout
    {
        bool Button(const String& label, const Size& size, bool border)
        {
            return ImGui::Button(label.c_str(), size);
        }
    }
    

//    Button::Button(std::string&& label):
//        Widget(std::move(label)),
//        m_pressed(false),
//        m_hovered(false),
//        m_held(false)
//    {
//        // This is neede due to a bug on the button renderer.
//        // Of course would be better to fix that, but whatever.
//        m_label = m_label + " ";
//    }
//
//    Button::~Button() {}
//
//    void Button::Draw(Vec2&& position, Vec2&& size)
//    {
////        ImGuiState& g = *((ImGuiState*)ImGui::GetInternalState());
////        const ImGuiStyle& style = g.Style;
////        
////        // Render
////        ImVec2 pos = m_position + style.FramePadding * 2;
////        ImVec2 correctSize = m_size - style.FramePadding * 4;
////        bndToolButton(ImGui::GetNVGcontext(), pos.x, pos.y, correctSize.x, correctSize.y, BND_CORNER_NONE, (m_hovered && m_held) ? BND_ACTIVE : m_hovered ? BND_HOVER : BND_DEFAULT, -1, m_label.c_str());
//        
//    }
//    
//    bool Button::HandleEvents(Vec2&& position, Vec2&& size)
//    {
//        ImGui::BeginChild(("Child" + m_label).c_str(), size);
//        ImGui::Button(m_label.c_str(), size);
//        ImGui::EndChild();
////        ImGuiWindow* window = ImGui::GetCurrentWindow();
////        
////        ImGuiButtonFlags flags = 0;
////        ImGuiState& g = *((ImGuiState*)ImGui::GetInternalState());
////        const ImGuiStyle& style = g.Style;
////        const ImGuiID id = window->GetID(m_label.c_str());
////        const ImVec2 label_size = ImGui::CalcTextSize(m_label.c_str(), NULL, true);
////        
////        ImVec2 pos = position + style.FramePadding * 2;
////        ImVec2 correctSize = (size - style.FramePadding * 4).NonNegative();
////        
////        if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)
////            pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
////        ImVec2 actualSize = ImGui::CalcItemSize(correctSize, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
////        
////        const ImRect bb(pos, pos + actualSize);
////        ImGui::ItemSize(bb, style.FramePadding.y);
////        if (!ImGui::ItemAdd(bb, &id))
////            return false;
////        
////        if (window->DC.ButtonRepeat) {
////            flags |= ImGuiButtonFlags_Repeat;
////        }
////        m_pressed = ImGui::ButtonBehavior(bb, id, &m_hovered, &m_held, true, flags);
////        
////        // Update position and size
////        m_position = position;
////        m_size     = bb.Max - bb.Min + style.FramePadding * 4;
//        
//        return Widget::HandleEvents(std::move(position), std::move(size));
//    }

} /* OCGui */

#endif
