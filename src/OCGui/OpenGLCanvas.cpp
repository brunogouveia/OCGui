#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/OpenGLCanvas.h>

namespace OCGui
{
    OpenGLCanvas* currentCanvas = NULL;
    static void callback(const ImDrawList* parent_list, const ImDrawCmd* cmd)
    {
        if (currentCanvas) {
            Vec2 min = ((ImVec2*)cmd->UserCallbackData)[0];
            Vec2 max = ((ImVec2*)cmd->UserCallbackData)[1];

            if (currentCanvas->GetDrawCallback()) {
                currentCanvas->GetDrawCallback()(min, max);
            }

            delete (Vec2*)cmd->UserCallbackData;
        }
    }

    OpenGLCanvas::OpenGLCanvas(std::string&& label, int width, int height):
        Widget(std::move(label)),
        m_drawCallback(NULL),
        m_width(width),
        m_height(height)
    {

    }

    OpenGLCanvas::~OpenGLCanvas() {}

    void OpenGLCanvas::Draw()
    {
        // Set current Canvas
        currentCanvas = this;
        ImGui::Canvas(m_label.c_str(), ImVec2(m_width, m_height), callback);

    }

} /* OCGui */

#endif
