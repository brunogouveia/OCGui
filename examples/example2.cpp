#include <OCGui/InputText.h>
#include <OCGui/Layout.h>
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
//    OCGui::Window*             window = new OCGui::Window("Main Window", 1280, 720);
//    OCGui::VBox*               vbox   = new OCGui::VBox("Main content", window);
//    OCGui::VBox*               vbox2  = new OCGui::VBox("Inner layout", vbox);
//    OCGui::FrameLayout*      flayout  = new OCGui::FrameLayout("Framelayout", window);
//    window->SetContent(vbox);
////    OCGui::Text*               text   = new OCGui::Text("Text test");
//    OCGui::Button*             button = new OCGui::Button("Button testssom");
//    OCGui::Button*            button2 = new OCGui::Button("Button testssom2");
//    OCGui::Button*            button3 = new OCGui::Button("Button testssom3");
//    OCGui::OpenGLCanvas*       canvas = new OCGui::OpenGLCanvas("Janelinha", 400, 200);
//    OCGui::VBox*          vboxcanvas  = new OCGui::VBox("Canvas layout", canvas);
//    canvas->SetContent(vboxcanvas);
//    OCGui::InputText*         console = new OCGui::InputTextMultiline("Inputssom2");
//    OCGui::InputText*         console2 = new OCGui::InputText("Inputssom3");
//    OCGui::InputText* input  = new OCGui::InputText("Inputssom"/*, OCGui::InputTextFlags_KeepActiveOnEnter, [&] {
////        static bool f = true;
//        std::cout << "Input content: " << std::string(input->GetBuffer()) << std::endl;
////        console->SetText(std::string(input->GetBuffer()));
//        input->ClearText();
//        
//        
////        if (f) {
////            input2->SetFlags(OCGui::InputTextFlags_ReadOnly);
////        } else {
////            input2->SetFlags(OCGui::InputTextFlags_None);
////        }
////        f = !f;
//
//    }, 2048*/);
//
//    canvas->SetDrawCallback([&] (ImVec2 min, ImVec2 max)
//    {
//        glEnable(GL_SCISSOR_TEST);
//        OCGui::Vec2 winSize = window->GetWindowSize();
//        OCGui::Vec2 fbSize = window->GetFramebufferSize();
//        // int winWidth = winSize.x, winHeight = winSize.y;
//    	int fbWidth = fbSize.x, fbHeight = fbSize.y;
//    	float pxRatio = fbSize.y / winSize.y;
//
//        min.x *= pxRatio;
//        min.y *= pxRatio;
//        max.x *= pxRatio;
//        max.y *= pxRatio;
//        //
//        glScissor(min.x, fbHeight - max.y, max.x - min.x, max.y - min.y);
//        glClearColor(1,0,0,1);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glClearColor(114, 114, 154, 1);
//        glViewport(min.x, fbHeight - max.y, max.x - min.x, max.y - min.y);
//        glDisable(GL_SCISSOR_TEST);
//        // Create Vertex Array Object
//        GLuint vao;
//        glGenVertexArrays(1, &vao);
//        glBindVertexArray(vao);
//
//        // std::cout << "vao: " << vao << std::endl;
//
//        // Create a Vertex Buffer Object and copy the vertex data to it
//        GLuint vbo;
//        glGenBuffers(1, &vbo);
//
//        GLfloat vertices[] = {
//            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
//             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
//             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
//            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
//        };
//
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//        // Create an element array
//        GLuint ebo;
//        glGenBuffers(1, &ebo);
//
//        GLuint elements[] = {
//            0, 1, 2,
//            2, 3, 0
//        };
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
//
//        // Create and compile the vertex shader
//        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//        glShaderSource(vertexShader, 1, &vertexSource, NULL);
//        glCompileShader(vertexShader);
//
//        // Create and compile the fragment shader
//        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//        glCompileShader(fragmentShader);
//
//        // Link the vertex and fragment shader into a shader program
//        GLuint shaderProgram = glCreateProgram();
//        glAttachShader(shaderProgram, vertexShader);
//        glAttachShader(shaderProgram, fragmentShader);
//        glBindFragDataLocation(shaderProgram, 0, "outColor");
//        glLinkProgram(shaderProgram);
//        glUseProgram(shaderProgram);
//
//        // Specify the layout of the vertex data
//        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
//        // std::cout << "posAttrib: " << posAttrib << std::endl;
//        // std::cout << "posAttrib: " << glGetAttribLocation(shaderProgram, "color") << std::endl;
//        glEnableVertexAttribArray(posAttrib);
//        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
//
//        GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
//        glEnableVertexAttribArray(colAttrib);
//        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
//        // Clear the screen to black
//        // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//        // glClear(GL_COLOR_BUFFER_BIT);
//
//        // Draw a rectangle from the 2 triangles using 6 indices
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//        glBindVertexArray(0);
//        glViewport(0,0,fbWidth,fbHeight);
//    });
//
//    //vbox->AddWidget(button);
//    vbox->AddWidget(input);
////    vbox->AddWidget(console);
////    vbox->AddWidget(console2);
//    vbox->AddWidget(flayout);
//    
//
////    flayout->AddWidget(button2);
//    
//    flayout->AddWidget(console);
////    flayout->AddWidget(button);
//
//
////
////    vbox->AddWidget(button);
////    vbox2->AddWidget(canvas);
////
////    vboxcanvas->AddWidget(button3);
////
//    //button->SetRelativeSize(OCGui::Vec2(100,10));
//    input->SetRelativeSize(OCGui::Vec2(100,10));
//    flayout->SetRelativeSize(OCGui::Vec2(100,50));
//    button->SetRelativeSize(OCGui::Vec2(20,20));
//    button->SetRelativeSize(OCGui::Vec2(30,30));
////    console2->SetRelativeSize(OCGui::Vec2(30,100));
////    canvas->SetRelativeSize(OCGui::Vec2(100,90));
////    vbox2->SetRelativeSize (OCGui::Vec2(100, 50));
////
////    button2->SetRelativeSize(OCGui::Vec2(100, 50));
////    canvas->SetRelativeSize (OCGui::Vec2(100, 50));
//    
//    vboxcanvas->SetRelativeSize(OCGui::Vec2(100,100));
//    button3->SetRelativeSize(OCGui::Vec2(100,10));
//    
////    vbox->AddWidget(canvas);
////    vbox->AddWidget(console);
////    vbox->AddWidget(input);
////    
////    canvas->SetRelativeSize(OCGui::Vec2(100, 80));
////    console->SetRelativeSize(OCGui::Vec2(100, 10));
////    input->SetRelativeSize(OCGui::Vec2(100,10));
    using namespace OCGui;
    
    OCGui::Size windowSize(1280, 720);
    OCGui::Init("OCGui", windowSize);
    
    
    
    char buffer[1000];
    char buffer2[1000];
    bool showCanvas2 = true;
    while (!OCGui::IsCloseRequested())
    {
        OCGui::HandlGLFWEvents();
        OCGui::BeginFrame();
        OCGui::Rect canvasRect = OCGui::Rect(ImGui::GetCursorStartPos(), (ImGui::GetContentRegionMax() * OCGui::Size(1.0, 0.5)));
        OCGui::Rect textRect   = OCGui::Rect((ImGui::GetContentRegionMax() * OCGui::Size(0.0, 0.5)) + ImGui::GetCursorStartPos(), (ImGui::GetContentRegionMax() * OCGui::Size(0.0, 0.5)) + (ImGui::GetContentRegionMax() * OCGui::Size(1.0, 0.5)));
        {
            BeginArea(ImGui::GetWindowSize() - Size(16,16));
            
            {
                BeginArea(GetCurrentAreaSize() * ImVec2(0.5, 0.1));
                if (Layout::Button("Canvas", (GetCurrentAreaSize() * ImVec2(1,1)) - ImGui::GetStyle().ItemSpacing * 0.5))
                {
                    std::cout << "Dr" << std::endl;
                    showCanvas2 = !showCanvas2;
                }
                EndArea();
                
                if (showCanvas2)
                {
                    ImGui::SameLine();
                    
                    BeginArea(GetCurrentAreaSize() * ImVec2(0.5, 0.1));
                    ImGui::Button("Canvas2", (GetCurrentAreaSize() * ImVec2(1,1)) - ImGui::GetStyle().ItemSpacing * 0.5);
                    EndArea();
                }
                
                
                BeginArea(GetCurrentAreaSize() * ImVec2(0.5, 0.1));
                Layout::Button("Canvas3", (GetCurrentAreaSize() * ImVec2(1,1)) - ImGui::GetStyle().ItemSpacing * 0.5);
                EndArea();
                BeginArea(GetCurrentAreaSize() * ImVec2(0.5, 0.1));
                Layout::Button("Canvas4", (GetCurrentAreaSize() * ImVec2(1,1)) - ImGui::GetStyle().ItemSpacing * 0.5);
                EndArea();
                BeginArea(GetCurrentAreaSize() * ImVec2(0.5, 0.1));
                Layout::Button("Canvas5", (GetCurrentAreaSize() * ImVec2(1,1)) - ImGui::GetStyle().ItemSpacing * 0.5);
                EndArea();
                BeginArea(GetCurrentAreaSize() * ImVec2(0.5, 0.1));
                Layout::Button("Canvas6", (GetCurrentAreaSize() * ImVec2(1,1)) - ImGui::GetStyle().ItemSpacing * 0.5);
                EndArea();
                BeginArea(GetCurrentAreaSize() * ImVec2(0.5, 0.1));
                Layout::Button("Canvas7", (GetCurrentAreaSize() * ImVec2(1,1)) - ImGui::GetStyle().ItemSpacing * 0.5);
                EndArea();
                
                BeginArea(GetCurrentAreaSize() * ImVec2(0.5, 0.4));
                //Layout::InputTextMultiline("Input", (GetCurrentAreaSize() * ImVec2(1,1)) - ImGui::GetStyle().ItemSpacing * 0.5, buffer, 1000);
                
                Layout::OpenGLCanvas("OpenGLCanvas", (GetCurrentAreaSize() * ImVec2(1,1)) - ImGui::GetStyle().ItemSpacing * 0.5, [] (Vec2 min, Vec2 max)
                {
//                    std::cout << "Drawing canvas" << std::endl;
                    glClearColor(0,0,0,1);
                    glClear(GL_COLOR_BUFFER_BIT);
                
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
                    
                    glBindVertexArray(0);
                });
                
                EndArea();
            }
            
            EndArea();
        }
        
        OCGui::EndFrame();
        OCGui::SwapBuffers();
    }
    
    OCGui::ShutDown();

//    delete input;
////    delete canvas;
////    delete button;
////    delete button2;
////    delete text;
////    delete vbox2;
//    delete vbox;
//    delete window;

    return 0;
}
