#include <OCGui/Window.h>
#include <OCGui/Text.h>
#include <OCGui/Button.h>
#include <OCGui/OpenGLCanvas.h>

int main(int argc, char const *argv[]) {
    OCGui::Window* window = new OCGui::Window("Main Window", 1280, 720);
    window->AddChild(new OCGui::Text("Text test"));
    window->AddChild(new OCGui::Button("Button test"));
    window->AddChild(new OCGui::OpenGLCanvas("Janelinha", 400, 200));

    while (!window->IsCloseRequested())
    {
        window->HandlEvents();
        window->Draw();
        window->SwapBuffers();
    }

    return 0;
}
