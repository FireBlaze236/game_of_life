#include <iostream>

#include <map>
#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp> 

#include "Window.h"
#include "Shader.h"
#include "Board.h"


template<typename T>
std::vector<T> flatten(std::vector<std::vector<T>>& orig)
{
    std::vector<T> v;
    for (const auto& x : orig)
    {
        v.insert(v.end(), x.begin(), x.end());
    }

    return v;
}



Board board(20, 20);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        board.Update();
        board.Debug();
    }
}

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
    glfwSetKeyCallback(window->GetPtr(), key_callback);

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

    board.SetCellActive(2, 1, true);
    board.SetCellActive(2, 2, true);
    board.SetCellActive(2, 3, true);
    board.SetCellActive(5, 5, true);
    board.SetCellType(5, 5, CELL_TYPE::SOLID);

    board.Debug();

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> cols;
    std::map<CELL_TYPE, glm::vec3> col_map =
    {
        {CELL_TYPE::SOLID, glm::vec3(1.0f, 1.0f, 1.0f)},
        {CELL_TYPE::CONWAY, glm::vec3(0.0f, 1.0f, 0.0f)}
    };
    
    for (int i = 0; i < board.GetHeight(); i++)
    {
        for (int j = 0; j < board.GetWidth(); j++)
        {
            positions.push_back(
                glm::vec3(j, i, 0)
            );

            CELL_TYPE t = board.GetCellType(i, j);
            cols.push_back(col_map[t]);
        }
    }


    
    glm::mat4 proj = glm::ortho(0.0f, 20.0f, 20.0f, 0.0f);
    

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

        std::vector<int> vis = board.GetIntegerRepFlatAll();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        shader.SetUniformMat4f("proj", proj);
        shader.SetUniformVec3fv("cols", cols);
        shader.SetUniform1iv("visible", vis);
        shader.SetUniformVec3fv("positions", positions);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL,
            board.GetWidth() * board.GetHeight());

        window->SwapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}