#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/VBox.h>
#include <iostream>

namespace OCGui
{
    
    VBox::VBox(std::string&& label, Widget* parent):
        Layout(std::move(label), parent)
    {
        
    }
    
    VBox::~VBox()
    {
        
    }
    
    void VBox::Draw(Vec2&& position, Vec2&& size)
    {
        Vec2 initialPosition = position;
        Vec2 initialSize     = size;
        
        for (Widget* child: m_children)
        {
            Vec2 childSize = initialSize * (child->GetRelativeSize() / 100.0f);
            child->Draw(std::move(initialPosition), std::move(childSize));

            initialPosition.y += child->GetSize().y;
        }
    }
    
    bool VBox::HandleEvents(Vec2&& position, Vec2&& size)
    {
        Vec2 initialPosition = m_position = position;
        Vec2 initialSize     = m_size     = size;
        
        for (Widget* child: m_children)
        {
            Vec2 childSize = initialSize * (child->GetRelativeSize() / 100.0f);
            if (child->HandleEvents(std::move(initialPosition), std::move(childSize)))
            {
                return true;
            }

            initialPosition.y += child->GetSize().y;
        }
        
        return false;
    }
    
} /* OCGui */

#endif
