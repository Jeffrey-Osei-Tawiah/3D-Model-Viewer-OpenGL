#include "Shader.h"
#include <sstream>
#include <fstream>

#include <SDL3/SDL_log.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
	:programID(0)
{
	unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
	std::string shaderData = LoadShaderFile(vertexShaderPath);
	const char* vShaderData = shaderData.c_str();
	glShaderSource(vShader, 1, &vShaderData, NULL);

	if (!CompileShader(vShader, GL_VERTEX_SHADER))
	{
		SDL_Log("Failed to compile vertex shader\n");
		return;
	}
	unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderData = LoadShaderFile(fragmentShaderPath);
	const char* fShaderData = shaderData.c_str();
	glShaderSource(fShader, 1, &fShaderData, NULL);

	if (!CompileShader(fShader, GL_FRAGMENT_SHADER))
	{
		SDL_Log("Failed to compile fragment shader\n");
		return;
	}

	unsigned int ID = glCreateProgram();
	if (!LinkProgram(ID, vShader, fShader))
	{
		SDL_Log("Failed to link program\n");
		return;
	}

	programID = ID;
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	glUseProgram(programID);
}

void Shader::SetFloat(std::string name, float val)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), val);
}

void Shader::SetInt(std::string name, int val)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), val);
}

void Shader::SetMat4(std::string name, glm::mat4 val)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

std::string Shader::LoadShaderFile(std::string path)
{
	std::ifstream file(path);
	std::string fileData;

	if (file.is_open())
	{
		std::stringstream data;
		data << file.rdbuf();

		fileData = data.str();
		file.close();
		return fileData;
	}

	SDL_Log("Failed to load shader file: %s\n", path.c_str());
	return std::string("");
}

bool Shader::CompileShader(int shaderID, GLenum shaderType)
{
	glCompileShader(shaderID);

	int status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		int numBytes;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &numBytes);

		std::string infoLog(numBytes, '\0');
		glGetShaderInfoLog(shaderID, numBytes, NULL, &infoLog[0]);

		std::string shType = "";
		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			shType = "VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			shType = "FRAGMENT";
			break;
		}

		SDL_Log("ERROR::SHADER::%s : %s\n", shType.c_str(), infoLog.c_str());
		return false;
	}
	return true;
}

bool Shader::LinkProgram(unsigned int programID, unsigned int vShader, unsigned int fShader)
{
	glAttachShader(programID, vShader);
	glAttachShader(programID, fShader);

	glLinkProgram(programID);

	int status;

	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (!status)
	{
		int numBytes;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &numBytes);

		std::string infoLog(numBytes, '\0');
		glGetProgramInfoLog(programID, numBytes, NULL, &infoLog[0]);
		SDL_Log("ERROR::SHADER::PROGRAM : %s", infoLog.c_str());
		return false;
	}

	return true;
}
