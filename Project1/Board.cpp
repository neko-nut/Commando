#include "Board.h"
// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
GLuint Board::generateObjectBuffer() {
	// Genderate 1 generic buffer object, called VBO
	glGenBuffers(1, &VBO);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	return VBO;
}

void Board::linkCurrentBuffertoShader(GLuint shaderProgramID) {
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "position");
	GLuint textureID = glGetAttribLocation(shaderProgramID, "aTexCoord");
	// Have to enable this

	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(textureID);
	glVertexAttribPointer(textureID, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}
#pragma endregion VBO_FUNCTIONS
