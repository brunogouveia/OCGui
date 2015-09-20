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

        virtual void Draw();

    };

}

#endif
