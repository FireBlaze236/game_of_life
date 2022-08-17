#include "Window.h"


Window::Window(int width, int height, std::string title)
{
    this->width = width;
    this->height = height;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    m_WindowPtr = glfwCreateWindow(width, height, title.c_str(), 0, 0);
    if (!m_WindowPtr)
    {
        LOG("Window Creation failed!");
    }
}
Window::Window()
    : Window(640, 480, "Default")
{
    
}



Window::~Window()
{
}

GLFWwindow* Window::GetPtr()
{
    return m_WindowPtr;
}

void Window::MakeCurrent()
{
    glfwMakeContextCurrent(m_WindowPtr);

    //After getting the context init glad
    if (gladLoadGL())
        std::cout << "Loaded opengl version " << GLVersion.major << "." << GLVersion.minor << std::endl;
    else
    {
        std::cout << "GLAD LOAD ERROR!" << std::endl;
    }
}

bool Window::isClosing()
{
    return glfwWindowShouldClose(m_WindowPtr);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_WindowPtr);
}

int Window::GetWidth()
{
    return width;
}

int Window::GetHeight()
{
    return height;
}

