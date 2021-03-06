#include "Aim.h"
// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
GLuint Aim::generateObjectBuffer() {
	// Genderate 1 generic buffer object, called VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
	//glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
	//glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(GLfloat), vertices);
	return VBO;
}

void Aim::linkCurrentBuffertoShader(GLuint shaderProgramID) {
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	// Have to enable this
	unsigned int vao;
	glGenBuffers(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(positionID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	//glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
#pragma endregion VBO_FUNCTIONS
