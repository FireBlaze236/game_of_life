#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Util.h"

#define SCREEN_W 640
#define SCREEN_H 480

class Window
{
private:
	GLFWwindow* m_WindowPtr;
	int width, height;
public:
	Window(int width, int height, std::string title);
	Window();
	
	~Window();
public:
	GLFWwindow* GetPtr();

	void MakeCurrent();

	bool isClosing();

	void SwapBuffers();

	int GetWidth();
	int GetHeight();
};

