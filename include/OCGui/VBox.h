#ifndef VBOX_H__
#define VBOX_H__

#include <OCGui/Layout.h>

namespace OCGui
{

    class VBox: public Layout
    {
    public:
        VBox(std::string&& label, Widget* parent);
        virtual ~VBox();
        
        virtual void Draw(Vec2&& position, Vec2&& size);
        virtual bool HandleEvents(Vec2&& position, Vec2&& size);
        
    };
    
} /* OCGui */

#endif