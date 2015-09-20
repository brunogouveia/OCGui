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

        void Draw();

    };

} /* OCGui */

#endif
