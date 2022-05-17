#include "Shader.h"
#include <iostream>
void Shader::checkCompilerErrors(unsigned int shader, std::string type)
{
	int success;
	char infolog[1024] = {};
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infolog);
			fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type: %d\n%s\n-----------------------------------------------------\n", type, infolog);
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infolog);
			fprintf(stderr, "ERROR::PROGRAM_LINKING_ERROR of type: %d\n%s\n-------------------------------------------------------\n", type, infolog);
		}
	}
}

Shader::Shader(const char * vertexPath, const char * fragmentPath, const char * geometryPath)
{
	//std::cout << vertexPath << std::endl;
	std::string vertexCode;
	std::string fragmentCode;
    std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
    std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
        gShaderFile.open(geometryPath);
		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
        gShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
        geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		fprintf(stderr, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}


	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment, geometry;
	int success;
	char infoLog[512];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompilerErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompilerErrors(fragment, "FRAGMENT");

    // geometry Shader
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    checkCompilerErrors(geometry, "GEOMETRY");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
    glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompilerErrors(ID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
    glDeleteShader(geometry);

}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{

	glUseProgram(ID);
}

void Shader::setUniform(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setUniform(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);

}

void Shader::setUniform(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);

}

Shader* Shader::loadShader(char* path, char* naziv) {
    std::string sPath(path);
    std::string pathVert;
    std::string pathFrag;
    std::string pathGeom;

    pathVert.append(path, sPath.find_last_of("\\/") + 1);
    pathFrag.append(path, sPath.find_last_of("\\/") + 1);
    pathGeom.append(path, sPath.find_last_of("\\/") + 1);
    if (pathFrag[pathFrag.size() - 1] == '/') {
        pathVert.append("shaders/");
        pathFrag.append("shaders/");
        pathGeom.append("shaders/");
    }
    else if (pathFrag[pathFrag.size() - 1] == '\\') {
        pathVert.append("shaders\\");
        pathFrag.append("shaders\\");
        pathGeom.append("shaders\\");
    }
    else {
        std::cerr << "nepoznat format pozicije shadera";
        exit(1);
    }

    pathVert.append(naziv);
    pathVert.append(".vert");
    pathFrag.append(naziv);
    pathFrag.append(".frag");
    pathGeom.append(naziv);
    pathGeom.append(".geom");

    return new Shader(pathVert.c_str(), pathFrag.c_str(), pathGeom.c_str());
}