#pragma once
#include "Util.h"
class Shader
{
private:
	unsigned int m_RendererId;

	std::string vertexShaderSource, fragmentShaderSource;
	void CompileShaders();
	void LinkShaders();
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

public:
	void Bind();

};

