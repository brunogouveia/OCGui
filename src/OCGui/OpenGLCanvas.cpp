#include <OCGui/OpenGLCanvas.h>

namespace OCGui
{

    OpenGLCanvas::OpenGLCanvas(std::string&& label, int width, int height):
        Widget(std::move(label)),
        m_width(width),
        m_height(height)
    {

    }

    OpenGLCanvas::~OpenGLCanvas() {}

    void OpenGLCanvas::Draw()
    {

    }

} /* OCGui */
