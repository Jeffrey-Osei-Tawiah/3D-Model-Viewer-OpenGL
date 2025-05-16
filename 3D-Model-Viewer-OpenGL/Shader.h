#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader():programID(0){}
	Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
	void Use()const { glUseProgram(programID); }

	void SetFloat(std::string name, float val);
	void SetInt(std::string name, int val);
	void SetMat4(std::string name, glm::mat4 val);

private:
	std::string LoadShaderFile(std::string path);
	bool CompileShader(int shaderID, GLenum shaderType);
	bool LinkProgram(unsigned int programID, unsigned int vShader, unsigned int fShader);

	unsigned int programID;
};

