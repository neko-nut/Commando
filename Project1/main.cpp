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
#include "Mesh.h"
#include "Cube.h"
#include "Texture.h"


using namespace std;

int width = 800;
int height = 600;

GLfloat rotate_y = 0.0f;
Shader* meshShader;
Shader* cubeShader;
Shader* textureShader;
Mesh *humanMesh;
Mesh *ground;
Mesh *house;
Cube *cube;

Texture* text;

GLfloat transform_x = 0.0f;
GLfloat transform_y = 0.0f;
int state = 0;


void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.53f, 0.81f, 0.92f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//glUseProgram(cubeShader->ID);
	//glBindBuffer(GL_ARRAY_BUFFER, cube->VBO);
	//cube->linkCurrentBuffertoShader(cubeShader->ID);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(meshShader->ID);
	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(meshShader->ID, "model");
	int view_mat_location = glGetUniformLocation(meshShader->ID, "view");
	int proj_mat_location = glGetUniformLocation(meshShader->ID, "proj");


	// Root of the Hierarchy
	mat4 view = identity_mat4();
	if (state == 1) {
		view = rotate_x_deg(view, 30.0f);
	}
	
	//view = rotate_x_deg(view, 30.0f);
	view = translate(view, vec3(0.0, 0.0, -10.0f));
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	mat4 model = identity_mat4();
	model = translate(model, vec3(0.0f, -10.0f, 0.0f));
	model = scale(model, vec3(0.16f, 0.16f, 0.16f));

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	
	humanMesh->linkCurrentBuffertoShader(meshShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, humanMesh->mesh_data.mPointCount);
	model = scale(model, vec3(6.25f, 6.25f, 6.25f));

	// Set up the child matrix
	mat4 modelGround = identity_mat4();
	modelGround = translate(modelGround, vec3(0.0f, -0.17f, 0.0f));
	modelGround = translate(modelGround, vec3(transform_x, 0.0f, transform_y));
	modelGround = scale(modelGround, vec3(100.0f, 10.0f, 100.0f));
	 
	// Apply the root matrix to the child matrix
	modelGround = model * modelGround;
	// Update the appropriate uniform and draw the mesh again
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelGround.m);
	ground->linkCurrentBuffertoShader(meshShader->ID);
	text->Bind(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, ground->mesh_data.mPointCount);


	modelGround = scale(modelGround, vec3(0.01f, 0.1f, 0.01f));
	mat4 modelBarrier = identity_mat4();
	modelBarrier = scale(modelBarrier, vec3(0.7f, 0.7f, 0.7f));
	modelBarrier = translate(modelBarrier, vec3(0.5f, 0.0f, -10.0f));
	modelBarrier = modelGround * modelBarrier;

	// Update the appropriate uniform and draw the mesh again
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelBarrier.m);
	house->linkCurrentBuffertoShader(meshShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, house->mesh_data.mPointCount);


	glutSwapBuffers();
}


void updateScene() {

	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	// Rotate the model slowly around the y axis at 20 degrees per second
	rotate_y += 20.0f * delta;
	rotate_y = fmodf(rotate_y, 360.0f);

	// Draw the next frame
	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	meshShader = new Shader();
	meshShader->CompileShaders("../shades/simpleVertexShader.txt", "../shades/simpleFragmentShader.txt");
	cubeShader = new Shader();
	cubeShader->CompileShaders("../shades/cubeVertexShader.txt", "../shades/cubeFragmentShader.txt");
	textureShader = new Shader();
	textureShader->CompileShaders("../shades/textureVertexShader.txt", "../shades/textureFragmentShader.txt");
	// load mesh into a vertex buffer array
	humanMesh = new Mesh();
	humanMesh->generateObjectBufferMesh("../models/FinalBaseMesh.obj");
	cube = new Cube();
	cube->generateObjectBuffer();
	ground = new Mesh();
	ground->generateObjectBufferMesh("../models/ground.dae");
	house = new Mesh();
	house->generateObjectBufferMesh("../models/house.dae");
	text = new Texture(GL_TEXTURE_2D, "../textures/grass.JPG");
	text->Load();
	
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	if (key == 'w') {
		transform_y = transform_y + 0.1f;
	}
	if (key == 'a') {
		transform_x = transform_x + 0.1f;
	}
	if (key == 's') {
		transform_y = transform_y - 0.1f;
	}
	if (key == 'd') {
		transform_x = transform_x - 0.1f;
	}
	if (key == ' ') {
		if (state == 0) {
			state = 1;
		} else if (state == 1){
			state = 0;
		}
	}

	glutPostRedisplay();
}



int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Triangle");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
