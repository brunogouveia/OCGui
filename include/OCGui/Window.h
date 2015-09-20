#ifndef WINDOW_H__
#define WINDOW_H__

#include <OCGui/Widget.h>

namespace OCGui
{
    class Window: public Widget
    {
    public:
        Window(std::string&& label, int width, int height);
        virtual ~Window();

        virtual void Draw();

        inline void HandlEvents()      { glfwPollEvents(); }
        inline bool IsCloseRequested() { return glfwWindowShouldClose(m_window); }
        inline void SwapBuffers()      { glfwSwapBuffers(m_window); }

    protected:
        GLFWwindow* m_window;
        int         m_width;
        int         m_height;
    };
}

#endif
