#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
using namespace std;




class Shader
{
private:
	static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	static char* readShaderSource(const char* shaderFile);

public:
	GLuint ID;
	GLuint CompileShaders(const char* pVertexShaderText, const char* pFragmentShaderText);
};



