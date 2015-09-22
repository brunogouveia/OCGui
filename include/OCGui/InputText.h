#ifndef INPUT_TEXT_H__
#define INPUT_TEXT_H__

#include <OCGui/Widget.h>

namespace OCGui
{

    class InputText: public Widget
    {
    public:
        InputText (std::string&& label, uint bufferSize = 512);
        virtual ~InputText ();

        virtual void Draw();

    private:
        char* m_buffer;
        uint  m_bufferSize;

    };

} /* OCGui */

#endif
