#ifndef INPUT_TEXT_MULTLINE_H__
#define INPUT_TEXT_MULTLINE_H__

#include <OCGui/InputText.h>

namespace OCGui
{

    class InputTextMultiline: public InputText
    {
    public:
        InputTextMultiline (std::string&& label, InputTextFlags flags = InputTextFlags_None, std::function<void(void)> enterCallback = []{}, uint bufferSize = 512);
        virtual ~InputTextMultiline ();
    };

} /* OCGui */

#endif
