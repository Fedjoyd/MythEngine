#include "Ressources/Shader.h"
#include <iostream>
#include <fstream>

Ressources::Shader::Shader(const std::string& name, const char* p_vertexShader, const char* p_fragShader)
	: Ressource(name)
{
	Program = glCreateProgram();

	std::ifstream vertexFile(p_vertexShader);
	std::ifstream fragFile(p_fragShader);
	std::string vertexSource;
	std::string fragSource;
	std::string buffer;

	if (vertexFile.is_open())
	{
		while (std::getline(vertexFile, buffer))
			vertexSource += (buffer + '\n');
		vertexFile.close();
	}
	else
		fprintf(stderr, "Unable to open vertex file");

	if (fragFile.is_open())
	{
		while (std::getline(fragFile, buffer))
			fragSource += (buffer + '\n');
		fragFile.close();
	}
	else
		fprintf(stderr, "Unable to open frag file");

	GLuint VertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource.c_str());
	GLuint FragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragSource.c_str());

	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);

	glLinkProgram(Program);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

GLuint Ressources::Shader::CompileShader(GLenum ShaderType, const char* shaderStrs)
{
	GLuint Shader = glCreateShader(ShaderType);

	glShaderSource(Shader, 1, &shaderStrs, nullptr);
	glCompileShader(Shader);

	GLint CompileStatus;
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &CompileStatus);
	if (CompileStatus == GL_FALSE)
	{
		char Infolog[1024];
		glGetShaderInfoLog(Shader, sizeof(Infolog) / sizeof(Infolog[0]), nullptr, Infolog);
		fprintf(stderr, "Shader error: %s\n", Infolog);
	}

	return Shader;
}

void Ressources::Shader::use()
{
	glUseProgram(Program);
}
