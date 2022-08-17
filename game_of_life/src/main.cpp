#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp> 

#include "Window.h"
#include "Shader.h"

int main()
{

    // INIT
    if (!glfwInit())
    {
        std::cout << "GLFW load error!";
        return -1;
    }
    //Window
    std::unique_ptr<Window> window(new Window());
    window->MakeCurrent();


    //Data
    float vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 500.0f, 0.0f,
        500.0f, 0.0f, 0.0f
    };
    glm::mat4 proj = glm::ortho(0.0f, (float) window->GetWidth(), 0.0f, (float)window->GetHeight());
    

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    shader.Bind();

    

    //Main Loop
    while (!window->isClosing())
    {
        glBindVertexArray(vao);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.SetUniformMat4f("proj", proj);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window->SwapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}