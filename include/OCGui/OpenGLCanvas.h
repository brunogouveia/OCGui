#ifndef OPENGL_CANVAS_H__
#define OPENGL_CANVAS_H__

#include <OCGui/Widget.h>
#include <OCGui/Layoutable.h>
#include <functional>

namespace OCGui
{

    class OpenGLCanvas: public Widget, public Layoutable
    {
    public:
        OpenGLCanvas (std::string&& label, int width, int height);
        virtual ~OpenGLCanvas ();

        virtual void Draw(Vec2&& position, Vec2&& size);
        virtual bool HandleEvents(Vec2&& position, Vec2&& size);
        
        inline void SetDrawCallback(std::function<void(ImVec2 min, ImVec2 max)> drawCallback) { m_drawCallback = drawCallback; }
        inline std::function<void(ImVec2 min, ImVec2 max)> GetDrawCallback() { return m_drawCallback; }

    protected:
        std::function<void(ImVec2 min, ImVec2 max)> m_drawCallback;

        int m_width;
        int m_height;
    };

} /* OCGui */

#endif
