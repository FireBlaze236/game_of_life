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
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 3 , 2
    };

    std::vector<glm::vec3> positions;
    int w = 10, h = 10;
    float scale = 0.9f;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            positions.push_back(
                glm::vec3(j, i, 0)
            );
        }
    }


    
    glm::mat4 proj = glm::ortho(0.0f, 10.0f, 10.0f, 0.0f);
    

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    shader.Bind();

    

    //Main Loop
    while (!window->isClosing())
    {
        glBindVertexArray(vao);
        glClear(GL_COLOR_BUFFER_BIT);

        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        shader.SetUniformMat4f("proj", proj);
        /*for (int i = 0; i < positions.size(); i++)
        {
            shader.SetUniformVec3f("positions[" + std::to_string(i) + "]",
                positions[i]);
        }*/
        shader.SetUniformVec3fv("positions", positions);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
            NULL, w * h);
        window->SwapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}