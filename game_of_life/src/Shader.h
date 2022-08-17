#pragma once
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include "Util.h"


class Shader
{
private:
	unsigned int m_RendererId;

	std::string vertexShaderSource, fragmentShaderSource;
	std::string ParseShader(const std::string& inputFilePath);
	void CompileShaders();

	bool compiled = false;
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

public:
	void Bind();

};

