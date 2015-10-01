#include <OCGui/Widget.h>

namespace OCGui
{

    void Widget::Active()
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        const ImGuiID id = window->GetID(m_label.c_str());

        ImGui::SetActiveID(id);
    }

} /* OCGui */
