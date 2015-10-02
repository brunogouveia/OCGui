#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/Text.h>

namespace OCGui
{

    Text::Text(std::string&& label):
        Widget(std::move(label))
    {
        m_label = m_label + " ";
    }

    Text::~Text() {}

    void Text::Draw(Vec2&& position, Vec2&& size)
    {
//        // ImGui::Text("%s", m_label.c_str());
//        using namespace ImGui;
//
//        const char* text = m_label.c_str();
//        const char* text_begin = m_label.c_str();
//        const char* text_end = text_begin + strlen(text_begin);
//
//        ImGuiWindow* window = GetCurrentWindow();
//        if (window->SkipItems)
//            return;
//
//        ImGuiState& g = *GImGui;
//        const ImGuiStyle& style = g.Style;
//
//        const float wrap_pos_x = window->DC.TextWrapPos;
//        const bool wrap_enabled = wrap_pos_x >= 0.0f;
//        if (text_end - text > 2000 && !wrap_enabled)
//        {
//            // // Long text!
//            // // Perform manual coarse clipping to optimize for long multi-line text
//            // // From this point we will only compute the width of lines that are visible. Optimization only available when word-wrapping is disabled.
//            // // We also don't vertically center the text within the line full height, which is unlikely to matter because we are likely the biggest and only item on the line.
//            // const char* line = text;
//            // const float line_height = ImGui::GetTextLineHeight();
//            // const ImVec2 text_pos = window->DC.CursorPos + ImVec2(0.0f, window->DC.CurrentLineTextBaseOffset);
//            // const ImRect clip_rect = window->ClipRect;
//            // ImVec2 text_size(0,0);
//            //
//            // if (text_pos.y <= clip_rect.Max.y)
//            // {
//            //     ImVec2 pos = text_pos;
//            //
//            //     // Lines to skip (can't skip when logging text)
//            //     if (!g.LogEnabled)
//            //     {
//            //         int lines_skippable = (int)((clip_rect.Min.y - text_pos.y) / line_height);
//            //         if (lines_skippable > 0)
//            //         {
//            //             int lines_skipped = 0;
//            //             while (line < text_end && lines_skipped < lines_skippable)
//            //             {
//            //                 const char* line_end = strchr(line, '\n');
//            //                 line = line_end + 1;
//            //                 lines_skipped++;
//            //             }
//            //             pos.y += lines_skipped * line_height;
//            //         }
//            //     }
//            //
//            //     // Lines to render
//            //     if (line < text_end)
//            //     {
//            //         ImRect line_rect(pos, pos + ImVec2(ImGui::GetWindowWidth(), line_height));
//            //         while (line < text_end)
//            //         {
//            //             const char* line_end = strchr(line, '\n');
//            //             if (IsClippedEx(line_rect, NULL, false))
//            //                 break;
//            //
//            //             const ImVec2 line_size = CalcTextSize(line, line_end, false);
//            //             text_size.x = ImMax(text_size.x, line_size.x);
//            //             RenderText(pos, line, line_end, false);
//            //             if (!line_end)
//            //                 line_end = text_end;
//            //             line = line_end + 1;
//            //             line_rect.Min.y += line_height;
//            //             line_rect.Max.y += line_height;
//            //             pos.y += line_height;
//            //         }
//            //
//            //         // Count remaining lines
//            //         int lines_skipped = 0;
//            //         while (line < text_end)
//            //         {
//            //             const char* line_end = strchr(line, '\n');
//            //             if (!line_end)
//            //                 line_end = text_end;
//            //             line = line_end + 1;
//            //             lines_skipped++;
//            //         }
//            //         pos.y += lines_skipped * line_height;
//            //     }
//            //
//            //     text_size.y += (pos - text_pos).y;
//            // }
//            //
//            // ImRect bb(text_pos, text_pos + text_size);
//            // ItemSize(bb);
//            // ItemAdd(bb, NULL);
//        }
//        else
//        {
//            ImVec2 size_arg = ImVec2(0,0);
//
////            const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
//            const ImVec2 label_size = ImGui::CalcTextSize(m_label.c_str(), NULL, true);
//
//            ImVec2 pos = window->DC.CursorPos;
//            // if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)
//            //     pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
//            ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 3.0f, label_size.y + style.FramePadding.y * 2.0f);
//
//            const ImRect bb(pos, pos + size);
//            ItemSize(bb, style.FramePadding.y);
//            if (!ItemAdd(bb, NULL))
//                return;
//
//            // Render (we don't hide text after ## in this end-user function)
////            int width = bndLabelWidth(GetNVGcontext(), BND_ICON_NONE, text_begin);
////            int height = bndLabelHeight(GetNVGcontext(), BND_ICON_NONE, text_begin, width);
//
//            ImVec2 sizes = bb.Max - bb.Min;
//            // std::cout << "Width : " << width << " - " << bb.Max.x - bb.Min.x << std::endl;
//            // std::cout << "Height: " << height << " - " << bb.Max.y - bb.Min.y << std::endl;
//            // RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
//            bndLabel(ImGui::GetNVGcontext(), bb.Min.x, bb.Min.y, sizes.x, sizes.y, -1, text_begin);
//        }

    }
    
    bool Text::HandleEvents(Vec2&& position, Vec2&& size)
    {
        return false;
    }

}

#endif
