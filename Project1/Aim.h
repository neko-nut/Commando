#pragma once
// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>
// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
class Aim
{
private:
	
	GLuint numVertices = 8;
	float vertices[24] = {
		 0.03f,  0.005f, 0.0f,// top right
		 0.03f, -0.005f, 0.0f, // bottom right
		-0.03f, -0.005f, 0.0f, // bottom left
		-0.03f,  0.005f, 0.0f, // top left

		 0.005f,  0.03f, 0.0f,// top right
		 0.005f, -0.03f, 0.0f, // bottom right
		-0.005f, -0.03f, 0.0f, // bottom left
		-0.005f,  0.03f, 0.0f, // top left 
	};

public:
	GLuint VBO;
	GLuint generateObjectBuffer();
	void linkCurrentBuffertoShader(GLuint shaderProgramID);

};

