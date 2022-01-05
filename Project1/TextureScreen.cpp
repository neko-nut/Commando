#include "TextureScreen.h"
#pragma region VBO_FUNCTIONS
GLuint TextureScreen::generateObjectBuffer() {
	// Genderate 1 generic buffer object, called VBO
	glGenBuffers(1, &VBO);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	return VBO;
}

void TextureScreen::linkCurrentBuffertoShader(GLuint shaderProgramID) {
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "position");
	GLuint normalID = glGetAttribLocation(shaderProgramID, "normal");
	// Have to enable this
	GLuint vao = 0;
	//glGenBuffers(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(positionID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(normalID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}
#pragma endregion VBO_FUNCTIONS
