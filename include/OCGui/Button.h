#ifndef BUTTON_H__
#define BUTTON_H__

#include <OCGui/Widget.h>

namespace OCGui
{

    class Button: public Widget
    {
    public:
        Button (std::string&& label);
        virtual ~Button ();

        virtual void Draw(Vec2&& position, Vec2&& size);
        virtual bool HandleEvents(Vec2&& position, Vec2&& size);
        
    protected:
        bool m_pressed;
        bool m_hovered;
        bool m_held;

    };

} /* OCGui */

#endif
