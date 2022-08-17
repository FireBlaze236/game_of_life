#pragma once
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/matrix.hpp>

#include "Util.h"


class Shader
{
private:
	unsigned int m_RendererId;

	std::string vertexShaderSource, fragmentShaderSource;
	std::string ParseShader(const std::string& inputFilePath);
	void CompileShaders();

	bool compiled = false;

	std::unordered_map<std::string, unsigned int> uniformMap;
	unsigned int GetUniformLocation(const std::string& name);
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

public:
	void Bind();

	void SetUniformMat4f(const std::string& uniformName, glm::mat4& mat);
	void SetUniformVec4f(const std::string& uniformName, glm::vec4& vec);
	void SetUniformVec4f(const std::string& uniformName, float a, float b, float c, float d);
};

