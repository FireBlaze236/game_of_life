#include <iostream>
#include <memory>
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



std::vector<std::vector<int>> visible = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        
};

int checkNeighbour(int i, int j, int w, int h)
{
    if (i < 0 || i >= h) return 0;
    if (j < 0 || j >= w) return 0;

    return visible[i][j];
}

void update()
{
    std::vector<std::vector<int>> nv;
    for (int i = 0; i < visible.size(); i++)
    {

        std::vector<int> v;
        for (int j = 0; j < visible[i].size(); j++)
        {
            int cnt = 0;
            cnt += checkNeighbour(i + 1, j, visible.size(), visible[i].size());
            cnt += checkNeighbour(i - 1, j, visible.size(), visible[i].size());
            cnt += checkNeighbour(i, j + 1, visible.size(), visible[i].size());
            cnt += checkNeighbour(i, j - 1, visible.size(), visible[i].size());

            cnt += checkNeighbour(i+1, j + 1, visible.size(), visible[i].size());
            cnt += checkNeighbour(i-1, j - 1, visible.size(), visible[i].size());
            cnt += checkNeighbour(i+1, j - 1, visible.size(), visible[i].size());
            cnt += checkNeighbour(i-1, j + 1, visible.size(), visible[i].size());

            if (visible[i][j] == 1)
            {
                if (cnt == 2 || cnt == 3)
                {
                    v.push_back(1);
                }
                else
                {
                    v.push_back(0);
                }
            }
            else if (visible[i][j] == 0)
            {
                if (cnt == 3)
                {
                    v.push_back(1);
                }
                else
                {
                    v.push_back(0);
                }
            }

            
            //std::cout << cnt << " ";
        }

        nv.push_back(v);
        //std::cout << std::endl;
    }

    visible = nv;
    
}

Board board(10, 10);

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

    std::vector<glm::vec3> positions;
    
    int w = 10, h = 10;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            positions.push_back(
                glm::vec3(j, i, 0)
            );

            //visible.push_back(1);
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

    
    board.SetCellActive(2, 1, true);
    board.SetCellActive(2, 2, true);
    board.SetCellActive(2, 3, true);
    board.Debug();
    


    //Main Loop
    while (!window->isClosing())
    {
        glBindVertexArray(vao);
        glClear(GL_COLOR_BUFFER_BIT);

        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        shader.SetUniformMat4f("proj", proj);
        auto varr = board.GetIntegerRep(CELL_TYPE::CONWAY);
        auto vis = flatten(varr);
        shader.SetUniform1iv("visible", vis);
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