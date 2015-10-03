#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/Widget.h>

namespace OCGui
{

    static Widget* m_childBeingDragged = NULL;

    bool Widget::HandleEvents(Vec2&& position, Vec2&& size)
    {
        if (ImGui::IsItemHovered()) {
            if (ImGui::IsMouseClicked(0)) {
                OnMouseClick(MouseEvent());
                return true;
            }
            else if (ImGui::IsMouseDown(0)) {
                OnMouseDown(MouseEvent());
                return true;
            }
            else if (ImGui::IsMouseReleased(0))
            {
                OnMouseRelease(MouseEvent());
                return true;
            }

            // Check for drop event
            if(m_childBeingDragged)
            {
                if (m_childBeingDragged != this)
                    OnDrop(DropEvent(m_childBeingDragged));
                m_childBeingDragged = NULL;
            }

        }

        if (ImGui::IsItemActive() && ImGui::IsMouseDragging()) {
            OnDrag(DragEvent(ImGui::GetMouseDragDelta()));
            m_childBeingDragged = this;
            return true;
        }

        return false;
    }

} /* OCGui */

#endif
