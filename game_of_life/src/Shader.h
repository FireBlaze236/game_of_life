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
	void SetUniformVec3f(const std::string& uniformName, glm::vec3& vec);
	void SetUniform1f(const std::string& uniformName, float val);

	void SetUniformVec3fv(const std::string& uniformName, std::vector<glm::vec3>& val);
	void SetUniform1iv(const std::string& uniformName, std::vector<int>& vals);

	void SetUniformMat4f(const std::string& uniformName, glm::mat4& mat);
	void SetUniformVec4f(const std::string& uniformName, glm::vec4& vec);
	void SetUniformVec4f(const std::string& uniformName, float a, float b, float c, float d);
};

