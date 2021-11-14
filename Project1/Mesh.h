#pragma once
// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations


// Project includes
#include "maths_funcs.h"
#include "Shader.h"


#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes

class Mesh
{
private:
	GLuint loc1, loc2, loc3;	
	ModelData load_mesh(const char* file_name);
	unsigned int vp_vbo;
	unsigned int vn_vbo;
	unsigned int vt_vbo;

public:
	ModelData mesh_data;
	void generateObjectBufferMesh(const char* mesh_name);
	void linkCurrentBuffertoShader(GLuint shaderProgramID);
};

