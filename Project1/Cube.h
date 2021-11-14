#pragma once
// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>
// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
class Cube
{
private:
	
	GLuint numVertices = 3;
	GLfloat vertices[9] = { -1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f, 1.0f, 0.0f };
	GLfloat colors[12] = { 0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f };
	
public:
	GLuint VBO;
	GLuint generateObjectBuffer();
	void linkCurrentBuffertoShader(GLuint shaderProgramID);

};

