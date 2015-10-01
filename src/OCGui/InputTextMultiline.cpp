#include <OCGui/InputTextMultiline.h>

namespace OCGui
{

    InputTextMultiline::InputTextMultiline(std::string&& label, InputTextFlags flags, std::function<void(void)> enterCallback, uint bufferSize):
        InputText(std::move(label), (InputTextFlags)(flags | InputTextFlags_Multiline), enterCallback, bufferSize)
    {

    }

    InputTextMultiline::~InputTextMultiline()
    {

    }

} /* OCGui */
