#include "TextureScreen.h"
#pragma region VBO_FUNCTIONS
GLuint TextureScreen::generateObjectBuffer() {
	// Genderate 1 generic buffer object, called VBO
	glGenBuffers(1, &VBO);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
	//glBufferData(GL_ARRAY_BUFFER, numVertices * 5 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
	//glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 5 * sizeof(GLfloat), vertices);
	//glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), numVertices * 4 * sizeof(GLfloat), colors);
	//glBufferSubData(GL_ARRAY_BUFFER, numVertices * 7 * sizeof(GLfloat), numVertices * 2 * sizeof(GLfloat), texture);
	return VBO;
}

void TextureScreen::linkCurrentBuffertoShader(GLuint shaderProgramID) {
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "position");
	GLuint normalID = glGetAttribLocation(shaderProgramID, "normal");
	GLuint textureID = glGetAttribLocation(shaderProgramID, "texCoord");
	// Have to enable this

	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(normalID);
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(textureID);
	glVertexAttribPointer(textureID, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}
#pragma endregion VBO_FUNCTIONS
