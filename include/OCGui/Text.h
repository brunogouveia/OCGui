#ifndef TEXT_H__
#define TEXT_H__

#include <OCGui/Widget.h>

namespace OCGui
{

    class Text: public Widget
    {
    public:
        Text (std::string&& label);
        virtual ~Text ();

        virtual void Draw(Vec2&& position, Vec2&& size);
        virtual bool HandleEvents(Vec2&& position, Vec2&& size);

    };

}

#endif
