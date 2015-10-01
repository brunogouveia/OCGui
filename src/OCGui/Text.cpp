#include <OCGui/Text.h>

namespace OCGui
{

    Text::Text(std::string&& label):
        Widget(std::move(label))
    {

    }

    Text::~Text() {}

    void Text::Draw()
    {
        ImGui::Text("%s", m_label.c_str());
    }

}
