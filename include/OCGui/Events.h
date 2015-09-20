#ifndef EVENTS_H__
#define EVENTS_H__

#include <ImGui/imgui.h>

namespace OCGui
{

    class MouseEvent
    {
    public:
        MouseEvent () {}
        virtual ~MouseEvent () {}
    };

    class DragEvent
    {
    public:
        DragEvent (ImVec2 dragDelta) : delta(dragDelta) {}
        virtual ~DragEvent () {}

        ImVec2 delta;
    };

    class Widget;

    class DropEvent
    {
    public:
        DropEvent(Widget* dropped) : widgetDropped(dropped) {}
        virtual ~DropEvent() {}

        Widget* widgetDropped;
    };

} /* OCGui */

#endif
