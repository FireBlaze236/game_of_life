#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Util.h"

class Window
{
private:
	GLFWwindow* m_WindowPtr;
public:
	Window(int width, int height, std::string title);
	Window();
	
	~Window();
public:
	GLFWwindow* GetPtr();

	void MakeCurrent();

	bool isClosing();

	void SwapBuffers();
};

