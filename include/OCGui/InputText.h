#ifndef INPUT_TEXT_H__
#define INPUT_TEXT_H__

#include <OCGui/Widget.h>

namespace OCGui
{
    // Flags for OCGui::InputText()
    typedef enum InputTextFlags_
    {
        // Default: 0
        InputTextFlags_None                = 0,        // No flag
        InputTextFlags_CharsDecimal        = 1 << 0,   // Allow 0123456789.+-*/
        InputTextFlags_CharsHexadecimal    = 1 << 1,   // Allow 0123456789ABCDEFabcdef
        InputTextFlags_CharsUppercase      = 1 << 2,   // Turn a..z into A..Z
        InputTextFlags_CharsNoBlank        = 1 << 3,   // Filter out spaces, tabs
        InputTextFlags_AutoSelectAll       = 1 << 4,   // Select entire text when first taking mouse focus
        InputTextFlags_EnterReturnsTrue    = 1 << 5,   // Return 'true' when Enter is pressed (as opposed to when the value was modified)
        InputTextFlags_CallbackCompletion  = 1 << 6,   // Call user function on pressing TAB (for completion handling)
        InputTextFlags_CallbackHistory     = 1 << 7,   // Call user function on pressing Up/Down arrows (for history handling)
        InputTextFlags_CallbackAlways      = 1 << 8,   // Call user function every time
        InputTextFlags_CallbackCharFilter  = 1 << 9,   // Call user function to filter character. Modify data->EventChar to replace/filter input, or return 1 to discard character.
        InputTextFlags_AllowTabInput       = 1 << 10,  // Pressing TAB input a '\t' character into the text field
        InputTextFlags_CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, allow exiting edition by pressing Enter. Ctrl+Enter to add new line (by default adds new lines with Enter).
        InputTextFlags_NoHorizontalScroll  = 1 << 12,  // Disable following the cursor horizontally
        InputTextFlags_AlwaysInsertMode    = 1 << 13,  // Insert mode
        InputTextFlags_ReadOnly            = 1 << 14,  // Read-only mode
        InputTextFlags_KeepActiveOnEnter   = 1 << 15,  // When enter is pressed, the widget will be still active
        // [Internal]
        InputTextFlags_Multiline           = 1 << 20   // For internal use by InputTextMultiline()
    } InputTextFlags;

    bool InputText(const String& label, const Rect& rect, char* buf, int buf_size);
    bool InputTextMultiline(const String& label, const Rect& rect, char* buf, int buf_size);
    
    
    namespace Layout
    {
        bool InputText(const String& label, const Size& rect, char* buf, int buf_size, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);
        bool InputTextMultiline(const String& label, const Size& rect, char* buf, int buf_size, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);
    }
    
//    class InputText: public Widget
//    {
//    public:
//        InputText (std::string&& label, InputTextFlags flags = InputTextFlags_None, std::function<void(void)> enterCallback = [](){}, uint bufferSize = 512);
//        virtual ~InputText ();
//
//        virtual void Draw(Vec2&& position, Vec2&& size);
//        virtual bool HandleEvents(Vec2&& position, Vec2&& size);
//        
//        virtual void ClearText();
//        virtual void SetText(std::string&& text);
//        virtual void SetFlags(InputTextFlags flags) { m_flags = flags; }
//        const char* GetBuffer() const { return m_buffer; }
//
//    protected:
//        InputTextFlags m_flags;
//        char*          m_buffer;
//        uint           m_bufferSize;
//
//        Vec2           m_renderPos;
//        Vec2           m_carotPos;
//        BNDwidgetState m_state;
//
//        std::function<void(void)> m_enterCallback;
//    };

} /* OCGui */

#endif
