#pragma once
#include <string>
#include <glad/glad.h>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~Shader();

	void Use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, float x, float y) const;


private:
	void CheckCompileErrors(GLuint shader, std::string type);
};

