#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/OpenGLCanvas.h>

namespace OCGui
{
    
    namespace Layout
    {
        typedef struct LayoutOpenGLCanvasCallbackData
        {
            Vec2 min;
            Vec2 max;
            Function<void(ImVec2 min, ImVec2 max)> drawCallback;
        } LayoutOpenGLCanvasCallbackData;
        
        static void LayoutOpenGLCanvasCallback(const ImDrawList*,const ImDrawCmd* cmd)
        {
            auto data = (LayoutOpenGLCanvasCallbackData*)cmd->UserCallbackData;

            if (data)
            {
                // Min and max in framebuffer coordinates, rather than window coordinates.
                Vec2 min = data->min * 2;
                Vec2 max = data->max * 2;
                
                Size framebufferSize = GetFramebufferSize();
                Vec2 position = Vec2(min.x, framebufferSize.y - max.y);
                Size size = max - min;
                
                glEnable(GL_SCISSOR_TEST);
                glScissor(position.x, position.y, size.x, size.y);
                glViewport(position.x, position.y, size.x, size.y);
                
                if (data->drawCallback)
                {
                    data->drawCallback(min, max);
                }
                delete data;
                
                glScissor(0, 0, framebufferSize.x, framebufferSize.y);
                glViewport(0, 0, framebufferSize.x, framebufferSize.y);
                glDisable(GL_SCISSOR_TEST);
            }
        }
        
        void OpenGLCanvas(const char* label, const ImVec2& size, Function<void(ImVec2 min, ImVec2 max)> drawCallback)
        {
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            if (window->SkipItems)
                return;
            
            ImGuiState& g = *GImGui;
            const ImGuiStyle& style = g.Style;
            const ImGuiID id = window->GetID(label);
            
            ImVec2 pos = window->DC.CursorPos;
            
            const ImRect bb(pos, pos + size);
            ImGui::ItemSize(bb, style.FramePadding.y);
            if (!ImGui::ItemAdd(bb, &id))
                return;
            
            // Behavior
            const bool hovered = ImGui::IsHovered(bb, id, true);
            if (hovered)
            {
                g.HoveredId = id;
                if ((!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
                {
                    if (g.IO.MouseClicked[0])
                    {
                        ImGui::SetActiveID(id, window);
                        ImGui::FocusWindow(window);
                    }
                }
            } else if (g.ActiveId == id) {
                if (g.IO.MouseClicked[0])
                {
                    ImGui::SetActiveID(0);
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
            
            // RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
            window->DrawList->AddCallback(LayoutOpenGLCanvasCallback, new LayoutOpenGLCanvasCallbackData{bb.Min, bb.Max, drawCallback});
        }
        
    }
    
}

//namespace OCGui
//{
//    typedef struct OpenGLCanvasCallbackData
//    {
//        OpenGLCanvas* canvas;
//        Vec2* bound;
//    } OpenGLCanvasCallbackData;
//    
//    static void OpenGLCanvasCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd)
//    {
//        OpenGLCanvasCallbackData* data = (OpenGLCanvasCallbackData*)cmd->UserCallbackData;
//        if (data) {
//            data->canvas->GetDrawCallback()(data->bound[0], data->bound[1]);
//            delete data->bound;
//            delete data;
//        }
//    }
//    
//    OpenGLCanvas::OpenGLCanvas(std::string&& label, int width, int height):
//        Widget(std::move(label)),
//        m_drawCallback(NULL),
//        m_width(width),
//        m_height(height)
//    {
//
//    }
//
//    OpenGLCanvas::~OpenGLCanvas() {}
//
//    void OpenGLCanvas::Draw(Vec2&& position, Vec2&& size)
//    {
//        if (m_content) {
//            m_content->Draw(std::move(position), std::move(size));
//        }
//        
////        if (GetDrawCallback())
////        {
////            GetDrawCallback()(std::move(m_position), m_position + m_size);
////        }
//        using namespace ImGui;
//        ImGuiWindow* window = GetCurrentWindow();
//        if (window->SkipItems)
//            return;
//        
//        Vec2* bound = new Vec2[2];
//        bound[0] = m_position;
//        bound[1] = m_position + m_size;
//        OpenGLCanvasCallbackData* data = new OpenGLCanvasCallbackData{this, bound};
//        window->DrawList->AddCallback(OpenGLCanvasCallback, data);
//        
//    }
//    
//    bool OpenGLCanvas::HandleEvents(Vec2&& position, Vec2&& size)
//    {
//        if (m_content && m_content->HandleEvents(std::move(position), std::move(size)))
//        {
//            return true;
//        }
//        
//        using namespace ImGui;
//        ImGuiWindow* window = GetCurrentWindow();
//        if (window->SkipItems)
//            return false;
//        
//        ImGuiState& g = *GImGui;
//        const ImGuiStyle& style = g.Style;
//        const ImGuiID id = window->GetID(m_label.c_str());
//        
//        Vec2 actualSize = Vec2(size.x ? size.x : m_width, size.y ? size.y : m_height);
//        
//        const ImRect bb(window->DC.CursorPos, position + actualSize);
//        m_position = window->DC.CursorPos;
//        ItemSize(bb, style.FramePadding.y);
//        if (!ItemAdd(bb, &id))
//            return false;
//        
//        // Behavior
//        const bool hovered = IsHovered(bb, id, true);
//        if (hovered)
//        {
//            g.HoveredId = id;
//            if ((!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
//            {
//                if (g.IO.MouseClicked[0])
//                {
//                    SetActiveID(id, window);
//                    FocusWindow(window);
//                }
//            }
//        } else if (g.ActiveId == id) {
//            if (g.IO.MouseClicked[0])
//            {
//                SetActiveID(0);
//            }
//        }
//        
//        bool held = false;
//        if (g.ActiveId == id)
//        {
//            g.ActiveIdIsFocusedOnly = !g.IO.MouseDown[0];
//            if (g.IO.MouseDown[0])
//            {
//                held = true;
//            }
//        }
//        
//        // Update position and size
////        m_position = window->DC.CursorPos;
//        m_size     = actualSize;
//
//        return Widget::HandleEvents(std::move(position), std::move(size));
//    }
//
//} /* OCGui */
//
#endif
