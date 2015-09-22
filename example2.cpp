#include <OCGui/Window.h>
#include <OCGui/Text.h>
#include <OCGui/InputText.h>
#include <OCGui/Button.h>
#include <OCGui/OpenGLCanvas.h>

// Shader sources
const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec2 position;"
    "in vec3 color;"
    "out vec3 Color;"
    "void main() {"
    "   Color = color;"
    "   gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 150 core\n"
    "in vec3 Color;"
    "out vec4 outColor;"
    "void main() {"
    "   outColor = vec4(Color, 1.0);"
    "}";

int main(int argc, char const *argv[]) {
    OCGui::Window*       window = new OCGui::Window("Main Window", 1280, 720);
    OCGui::Text*         text   = new OCGui::Text("Text test");
    OCGui::Button*       button = new OCGui::Button("Button testssom");
    OCGui::OpenGLCanvas* canvas = new OCGui::OpenGLCanvas("Janelinha", 400, 200);
    OCGui::InputText*    input  = new OCGui::InputText("Inputssom");

    canvas->SetDrawCallback([&] (ImVec2& min, ImVec2& max)
    {
        glEnable(GL_SCISSOR_TEST);
        OCGui::Vec2 winSize = window->GetWindowSize();
        OCGui::Vec2 fbSize = window->GetFramebufferSize();
        int winWidth = winSize.x, winHeight = winSize.y;
    	int fbWidth = fbSize.x, fbHeight = fbSize.y;
    	float pxRatio = fbSize.y / winSize.y;

        min.x *= pxRatio;
        min.y *= pxRatio;
        max.x *= pxRatio;
        max.y *= pxRatio;

        glScissor(min.x, fbHeight - max.y, max.x - min.x, max.y - min.y);
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(114, 114, 154, 1);
        glViewport(min.x, fbHeight - max.y, max.x - min.x, max.y - min.y);
        glDisable(GL_SCISSOR_TEST);
        // Create Vertex Array Object
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // std::cout << "vao: " << vao << std::endl;

        // Create a Vertex Buffer Object and copy the vertex data to it
        GLuint vbo;
        glGenBuffers(1, &vbo);

        GLfloat vertices[] = {
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Create an element array
        GLuint ebo;
        glGenBuffers(1, &ebo);

        GLuint elements[] = {
            0, 1, 2,
            2, 3, 0
        };

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

        // Create and compile the vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        // Create and compile the fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        // Link the vertex and fragment shader into a shader program
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glBindFragDataLocation(shaderProgram, 0, "outColor");
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        // Specify the layout of the vertex data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        // std::cout << "posAttrib: " << posAttrib << std::endl;
        // std::cout << "posAttrib: " << glGetAttribLocation(shaderProgram, "color") << std::endl;
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

        GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
        // Clear the screen to black
        // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);

        // Draw a rectangle from the 2 triangles using 6 indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glViewport(0,0,fbWidth,fbHeight);
    });

    window->AddChild(text);
    window->AddChild(button);
    window->AddChild(canvas);
    window->AddChild(input);

    while (!window->IsCloseRequested())
    {
        window->HandlEvents();
        window->Draw();
        window->SwapBuffers();
    }

    delete input;
    delete canvas;
    delete button;
    delete text;
    delete window;

    return 0;
}
