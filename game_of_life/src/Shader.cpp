#include "Shader.h"


std::string Shader::ParseShader(const std::string& inputFilePath)
{
	std::ifstream inputFile;
	inputFile.open(inputFilePath, std::ios::in);
	if (!inputFile.is_open())
	{
		LOG("ERROR OPENING SHADER FILE: " + inputFilePath);
		return "";
	}

	std::stringstream ss;
	std::string line;
	if (inputFile.is_open())
	{
		while (std::getline(inputFile, line))
		{
			ss << line << '\n';
		}
	}

	inputFile.close();

	return ss.str();
}

void Shader::CompileShaders()
{
	m_RendererId = glCreateProgram();
	unsigned int vertexShader, fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char* src = const_cast<char*>(vertexShaderSource.c_str());
	glShaderSource(vertexShader, 1, &src, NULL);
	glCompileShader(vertexShader);

	int status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLsizei log_length = 0;
		char message[1024];
		glGetShaderInfoLog(vertexShader, 1024, &log_length, message);

		GL_LOG(message);

		return;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	src = const_cast<char*>(fragmentShaderSource.c_str());
	glShaderSource(fragmentShader, 1, &src, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLsizei log_length = 0;
		char message[1024];
		glGetShaderInfoLog(fragmentShader, 1024, &log_length, message);

		GL_LOG(message);

		return;
	}

	glAttachShader(m_RendererId, vertexShader);
	glAttachShader(m_RendererId, fragmentShader);

	glLinkProgram(m_RendererId);

	glGetProgramiv(m_RendererId, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLsizei log_length = 0;
		char message[1024];
		glGetShaderInfoLog(m_RendererId, 1024, &log_length, message);

		GL_LOG(message);
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	vertexShaderSource = ParseShader(vertexShaderPath);
	fragmentShaderSource = ParseShader(fragmentShaderPath);

	CompileShaders();
}

void Shader::Bind()
{
	if (!compiled)
	{
		CompileShaders();
		compiled = true;
	}
	glUseProgram(m_RendererId);
}
