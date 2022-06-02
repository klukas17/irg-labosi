#pragma once

#include <glad/glad.h>

#include <string>

#include <fstream>
#include <sstream>

class Shader
{
private:
	void checkCompilerErrors(unsigned int shader, std::string type);

public:

	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, bool geom);

	~Shader();

	void use();
	void setUniform(const std::string &name, bool value)const;
	void setUniform(const std::string &name, int value)const;
	void setUniform(const std::string &name, float value)const;

    static Shader* loadShader(char* path, char* naziv, bool geom);
};

