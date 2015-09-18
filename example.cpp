// ImGui - standalone example application for Glfw + OpenGL 3, using programmable pipeline

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw_gl3.h>
#include <stdio.h>
#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

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


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL3 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    // Load Fonts
    // (see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    // Merge glyphs from multiple fonts into one (e.g. combine default font with another with Chinese glyphs, or add icons)
    //ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    //ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/fontawesome-webfont.ttf", 18.0f, &icons_config, icons_ranges);

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);
    char lol[100] = "";

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        ImGui::SetWindowPos(ImVec2(0,0));
        ImGui::SetWindowSize(ImVec2(1280, 720));

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::InputText("Hello input", lol, 100);
            // if (ImGui::IsItemHovered()) {
            //     std::cout << "/* message */" << std::endl;
            // }
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging()) {
                std::cout << "active" << std::endl;
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0)) {
                std::cout << "released" << std::endl;
            }
            if (ImGui::IsItemHoveredRect() && ImGui::IsMouseReleased(0)) {
                std::cout << "released" << std::endl;
            }

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;

            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // // 2. Show another simple window, this time using an explicit Begin/End pair
        // if (show_another_window)
        // {
        //     ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
        //     ImGui::Begin("Another Window", &show_another_window);
        //     ImGui::Text("Hello");
        //     ImGui::End();
        // }
        //
        // // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        // if (show_test_window)
        // {
        //     ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        //     ImGui::ShowTestWindow(&show_test_window);
        // }

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        // Create Vertex Array Object
  // GLuint vao;
  // glGenVertexArrays(1, &vao);
  // glBindVertexArray(vao);
  //
  // std::cout << "vao: " << vao << std::endl;
  //
  // // Create a Vertex Buffer Object and copy the vertex data to it
  // GLuint vbo;
  // glGenBuffers(1, &vbo);
  //
  // GLfloat vertices[] = {
  //     -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
  //      0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
  //      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
  //     -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
  // };
  //
  // glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  //
  // // Create an element array
  // GLuint ebo;
  // glGenBuffers(1, &ebo);
  //
  // GLuint elements[] = {
  //     0, 1, 2,
  //     2, 3, 0
  // };
  //
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  //
  // // Create and compile the vertex shader
  // GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // glShaderSource(vertexShader, 1, &vertexSource, NULL);
  // glCompileShader(vertexShader);
  //
  // // Create and compile the fragment shader
  // GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  // glCompileShader(fragmentShader);
  //
  // // Link the vertex and fragment shader into a shader program
  // GLuint shaderProgram = glCreateProgram();
  // glAttachShader(shaderProgram, vertexShader);
  // glAttachShader(shaderProgram, fragmentShader);
  // glBindFragDataLocation(shaderProgram, 0, "outColor");
  // glLinkProgram(shaderProgram);
  // glUseProgram(shaderProgram);
  //
  // // Specify the layout of the vertex data
  // GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  // std::cout << "posAttrib: " << posAttrib << std::endl;
  // std::cout << "posAttrib: " << glGetAttribLocation(shaderProgram, "color") << std::endl;
  // glEnableVertexAttribArray(posAttrib);
  // glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  //
  // GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  // glEnableVertexAttribArray(colAttrib);
  // glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  // // Clear the screen to black
  // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  // glClear(GL_COLOR_BUFFER_BIT);
  //
  //         // Draw a rectangle from the 2 triangles using 6 indices
  //         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
