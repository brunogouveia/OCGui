#ifndef OPENGL_CANVAS_H__
#define OPENGL_CANVAS_H__

#include <OCGui/Widget.h>

namespace OCGui
{

    class OpenGLCanvas: public Widget
    {
    public:
        OpenGLCanvas (std::string&& label, int width, int height);
        virtual ~OpenGLCanvas ();

        void Draw();

    protected:
        int m_width;
        int m_height;
    };

} /* OCGui */

#endif
