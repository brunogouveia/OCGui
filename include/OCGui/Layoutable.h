#ifndef LAYOUTABLE_H__
#define LAYOUTABLE_H__

#include <OCGui/Layout.h>

namespace OCGui
{

    class Layoutable
    {
    public:
        Layoutable (Layout* content = NULL): m_content(content) {}
        virtual ~Layoutable () {}
        
        inline Widget* GetContent()                { return m_content; }
        inline void    SetContent(Layout* content) { m_content = content; }

    protected:
        Layout* m_content;
    };

} /* OCGui */

#endif
