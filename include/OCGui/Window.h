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

        void Render();
        virtual void Draw(Vec2&& position, Vec2&& size);
        virtual bool HandleEvents(Vec2&& position, Vec2&& size);

        Vec2 GetFramebufferSize();
        Vec2 GetWindowSize();
        
        inline Widget* GetContent()                { return m_content; }
        inline void    SetContent(Widget* content) {m_content = content; }

        inline void HandlGLFWEvents()  { glfwPollEvents(); }
        inline bool IsCloseRequested() { return glfwWindowShouldClose(m_window); }
        inline void SwapBuffers()      { glfwSwapBuffers(m_window); }

    protected:
        GLFWwindow* m_window;
        int         m_width;
        int         m_height;
        Widget*     m_content;
        Widget*     m_childBeingDragged;
    };
}

#endif
