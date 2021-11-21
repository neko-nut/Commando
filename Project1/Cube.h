#pragma once
// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>
// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
class Cube
{
private:
	
	GLuint numVertices = 4;
	float vertices[12] = {
		 0.01f,  0.01f, 0.0f,// top right
		 0.01f, -0.01f, 0.0f, // bottom right
		-0.01f, -0.01f, 0.0f, // bottom left
		-0.01f,  0.01f, 0.0f, // top left 
	};
public:
	GLuint VBO;
	GLuint generateObjectBuffer();
	void linkCurrentBuffertoShader(GLuint shaderProgramID);

};

