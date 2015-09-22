#include <OCGui/InputText.h>

namespace OCGui
{

    InputText::InputText(std::string&& label, uint bufferSize):
        Widget(std::move(label)),
        m_buffer(new char[bufferSize]),
        m_bufferSize(bufferSize)
    {

    }

    InputText::~InputText()
    {
        if (m_buffer)
            delete m_buffer;
    }

    void InputText::Draw()
    {
        ImGui::InputText(m_label.c_str(), m_buffer, m_bufferSize);
    }


} /* OCGui */
