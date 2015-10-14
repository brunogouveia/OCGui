#ifndef LAYOUT_H__
#define LAYOUT_H__

#include <OCGui/Widget.h>

namespace OCGui
{
    
    void BeginArea(const Size& size);
    void EndArea();
    Size GetCurrentAreaSize();

    
    
//    class Layout: public Widget
//    {
//    public:
//        Layout (std::string&& label, Widget* parent): Widget(std::move(label)), m_parent(parent) {}
//        virtual ~Layout () {}
//
//        virtual void Draw(Vec2&& position, Vec2&& size) = 0;
//        virtual bool HandleEvents(Vec2&& position, Vec2&& size) = 0;
//        
//        inline void AddWidget(Widget* child) { m_children.push_back(child); }
//        
//    protected:
//        Widget*              m_parent;
//        std::vector<Widget*> m_children;
//
//    };

} /* OCGui */

#endif
