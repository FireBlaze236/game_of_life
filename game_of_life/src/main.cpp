#include <iostream>
#include <map>
#include <chrono>

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp> 
#include "glm/gtc/type_ptr.hpp"

#include "Window.h"
#include "Shader.h"
#include "Board.h"


#define TICKS_PER_SEC 60

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
bool running = false;
double delay = 0.1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_SPACE:
            running = !running;
            break;
        case GLFW_KEY_C:
            board.Clear();
            break;
        case GLFW_KEY_PERIOD:
            delay -= 0.02;
            delay = std::max(delay, 0.01);
            break;
        case GLFW_KEY_COMMA:
            delay += 0.02;
            delay = std::min(delay, 1.00);
            break;
        default:
            break;
        }
    }
    
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        

        double i = x / SCREEN_W;
        double j = y / SCREEN_H;
        i *= board.GetWidth();
        j *= board.GetHeight();
        int p = std::floor(i);
        int q = std::floor(j);
        //std::cout << std::floor(i) << " " << std::floor(j) << std::endl;


        board.SetCellActive(q, p, true);

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
    glfwSetMouseButtonCallback(window->GetPtr(), mouse_button_callback);

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


    
    glm::mat4 proj = glm::ortho(0.0f, (float)board.GetWidth(), (float)board.GetHeight(), 0.0f);
    

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    

    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_STATIC_READ);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1);

    unsigned int colorVBO;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cols.size(), &cols[0], GL_STATIC_READ);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    shader.Bind();

    
    using namespace std::chrono;
    
    auto prev = steady_clock::now();
    auto now = steady_clock::now();

    //Main Loop
    while (!window->isClosing())
    {
        glBindVertexArray(vao);
        glClear(GL_COLOR_BUFFER_BIT);

        std::vector<int> vis = board.GetIntegerRepFlatAll();
        for (int i = 0; i < vis.size(); i++ )
        {
            positions[i].z = vis[i];
        }
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        shader.SetUniformMat4f("proj", proj);
        
        
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL,
            board.GetWidth() * board.GetHeight());

        window->SwapBuffers();
        glfwPollEvents();

        //TIMING
        now = steady_clock::now();
        std::chrono::duration<double> diff = now - prev;
        if ( running && diff.count() >= delay)
        {
            prev = now;
            board.Update();
        }
    }

    glfwTerminate();
    return 0;
}