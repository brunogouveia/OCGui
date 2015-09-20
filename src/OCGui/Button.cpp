#include <OCGui/Button.h>

namespace OCGui {

    Button::Button(std::string&& label):
        Widget(std::move(label))
    {

    }

    Button::~Button() {}

    void Button::Draw()
    {
        ImGui::Button(m_label.c_str());
    }

} /* OCGui */
