#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

class Board
{

private:
	GLuint numVertices = 6;
	float vertices[30] = {
		// positions        // texture coords
		 0.7f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
		 0.7f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
		 -0.7f,  0.5f, 0.0f,  0.0f, 1.0f,  // top left 

		 -0.7f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
		 0.7f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
		 -0.7f,  0.5f, 0.0f,  0.0f, 1.0f,  // top left 
	};

public:
	GLuint VBO;
	GLuint generateObjectBuffer();
	void linkCurrentBuffertoShader(GLuint shaderProgramID);
};

