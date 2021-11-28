// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#include <stdlib.h> 

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
#include "TextureScreen.h"
#include "Texture.h"
#include "Game.h"


#define  PI 3.1415926

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

Texture* grass;
Texture* white;
Texture* red;
Texture* sight;
Game* game;
TextureScreen* test;

GLfloat transform_x = 0.0f;
GLfloat transform_y = 0.0f;
float rotate_x = 0.0f;

GLfloat enemy_x = 10.0f;
GLfloat enemy_y = 20.0f;


GLfloat enemy_x2 = -30.0f;
GLfloat enemy_y2 = 20.0f;

int viewstate = 0;
int enemystate = 0;
int enemystate2 = 0;




void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.53f, 0.81f, 0.92f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//glUseProgram(cubeShader->ID);
	//glBindBuffer(GL_ARRAY_BUFFER, test->VBO);
	//test->linkCurrentBuffertoShader(cubeShader->ID);
	//sight->Bind(GL_TEXTURE0);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	if (viewstate == 1) {
		glUseProgram(cubeShader->ID);
		glBindBuffer(GL_ARRAY_BUFFER, cube->VBO);
		cube->linkCurrentBuffertoShader(cubeShader->ID);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}


	glUseProgram(meshShader->ID);
	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(meshShader->ID, "model");
	int view_mat_location = glGetUniformLocation(meshShader->ID, "view");
	int proj_mat_location = glGetUniformLocation(meshShader->ID, "proj");


	// Root of the Hierarchy
	mat4 view = identity_mat4();
	
	view = rotate_y_deg(view, 180.0f);
	view = rotate_x_deg(view, 10.0f);
	view = translate(view, vec3(0.0, 0.0, -10.0f));
	if (viewstate == 1) {
		view = translate(view, vec3(0.0, 0.0, 10.18f));
		view = rotate_x_deg(view, -10.0f);
	}
	
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	mat4 model = identity_mat4();
	
	model = translate(model, vec3(0.0f, -10.0f, 0.0f));
	model = rotate_y_deg(model, rotate_x);
	mat4 humanGround = identity_mat4();
	humanGround = rotate_y_deg(humanGround, -rotate_x);
	humanGround = translate(humanGround, vec3(0.0f, 30.0f, 0.0f));
	humanGround = scale(humanGround, vec3(0.25f, 0.25f, 0.25f));
	
	humanGround = model * humanGround;

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, humanGround.m);
	
	humanMesh->linkCurrentBuffertoShader(meshShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, humanMesh->mesh_data.mPointCount);


	glUseProgram(textureShader->ID);
	//Declare your uniform variables that will be used in your shader
	int texture_matrix_location = glGetUniformLocation(textureShader->ID, "model");
	int texture_view_mat_location = glGetUniformLocation(textureShader->ID, "view");
	int texture_proj_mat_location = glGetUniformLocation(textureShader->ID, "proj");


	// Set up the child matrix
	mat4 modelGround = identity_mat4();
	modelGround = translate(modelGround, vec3(0.0f, -0.19f, 0.0f));
	modelGround = scale(modelGround, vec3(100.0f, 10.0f, 100.0f));
	modelGround = translate(modelGround, vec3(transform_x, 0.0f, transform_y));
	 
	// Apply the root matrix to the child matrix
	modelGround = model * modelGround;
	// Update the appropriate uniform and draw the mesh again
	glUniformMatrix4fv(texture_proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(texture_view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(texture_matrix_location, 1, GL_FALSE, modelGround.m);
	ground->linkCurrentBuffertoShader(textureShader->ID);
	grass->Bind(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, ground->mesh_data.mPointCount);
	
	
	mat4 modelBarrier = identity_mat4();
	modelBarrier = scale(modelBarrier, vec3(0.01f, 0.1f, 0.01f));
	modelBarrier = scale(modelBarrier, vec3(2.0f, 2.0f, 2.0f));
	modelBarrier = translate(modelBarrier, vec3(0.0f, 1.0f, 0.3f));

	modelBarrier = modelGround * modelBarrier;

	// Update the appropriate uniform and draw the mesh again
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelBarrier.m);
	white->Bind(GL_TEXTURE0);
	house->linkCurrentBuffertoShader(textureShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, house->mesh_data.mPointCount);


	//enemy_y = enemy_y - 0.01;
	mat4 modelEnemies = identity_mat4();
	modelEnemies = translate(modelEnemies, vec3(enemy_x, 0.0f, enemy_y));
	modelEnemies = scale(modelEnemies, vec3(0.01f, 0.1f, 0.01f));
	modelEnemies = scale(modelEnemies, vec3(0.25f, 0.25f, 0.25f));
	modelEnemies = translate(modelEnemies, vec3(0.0f, 0.935f, 0.0f));
	modelEnemies = modelGround * modelEnemies;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelEnemies.m);
	if (enemystate == 0) {
		red->Bind(GL_TEXTURE0);
		humanMesh->linkCurrentBuffertoShader(meshShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, humanMesh->mesh_data.mPointCount);
	}


	modelEnemies = identity_mat4();
	if (enemy_y2 > -30) {
		enemy_y2 = enemy_y2 - 0.01;
	}
	modelEnemies = translate(modelEnemies, vec3(enemy_x2, 0.0f, enemy_y2));
	modelEnemies = scale(modelEnemies, vec3(0.01f, 0.1f, 0.01f));
	modelEnemies = scale(modelEnemies, vec3(0.25f, 0.25f, 0.25f));
	modelEnemies = translate(modelEnemies, vec3(0.0f, 0.935f, 0.0f));
	modelEnemies = modelGround * modelEnemies;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelEnemies.m);
	if (enemystate2 == 0) {
		red->Bind(GL_TEXTURE0);
		humanMesh->linkCurrentBuffertoShader(meshShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, humanMesh->mesh_data.mPointCount);
	}



	glutSwapBuffers();

}

bool shoot() {
	float x = -(enemy_x + transform_x * 4);
	float y = enemy_y + transform_y * 4;

	float angle_x = cos((90 - rotate_x) * PI / 180.0f);
	float angle_y = sin((90 - rotate_x) * PI / 180.0f);

	float angle_cos = (x * angle_x + y * angle_y) / (sqrt(pow(x, 2) + pow(y, 2)) + sqrt(pow(angle_x, 2) + pow(angle_y, 2)));
	printf("angle 1:  %f  %f  %f  %f  %f  %f  %f  %f\n",x, y, angle_x, angle_y, angle_cos, 90 - rotate_x, acos((x / sqrt(pow(x, 2) + pow(y, 2))))* 180.0f / PI, acos(angle_cos)* 180.0f / PI);
	if (angle_cos > 0.95f) {
		return true;
	}


	return false;
}


bool shoot2() {
	float x = -(enemy_x2 + transform_x * 4);
	float y = enemy_y2 + transform_y * 4;

	float angle_x = cos((90 - rotate_x) * PI / 180.0f);
	float angle_y = sin((90 - rotate_x) * PI / 180.0f);

	float angle_cos = (x * angle_x + y * angle_y) / (sqrt(pow(x, 2) + pow(y, 2)) + sqrt(pow(angle_x, 2) + pow(angle_y, 2)));
	printf("angle 2:  %f  %f  %f  %f  %f  %f  %f  %f\n\n", x, y, angle_x, angle_y, angle_cos, 90 - rotate_x, acos((x / sqrt(pow(x, 2) + pow(y, 2))))* 180.0f / PI, acos(angle_cos)* 180.0f / PI);
	if (angle_cos > 0.95f) {
		return true;
	}


	return false;
}


bool collision() {
	int x = 15;
	int y = 12.5;

	
	if (transform_y < -30 + x && transform_x < y && transform_x > -y && transform_y > -30 - x) {
		return true;
	}

	return false;
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
	textureShader ->CompileShaders("../shades/textureVertexShader.txt", "../shades/textureFragmentShader.txt");
	// load mesh into a vertex buffer array
	humanMesh = new Mesh();
	humanMesh->generateObjectBufferMesh("../models/human.dae");
	cube = new Cube();
	cube->generateObjectBuffer();
	ground = new Mesh();
	ground->generateObjectBufferMesh("../models/ground.dae");
	house = new Mesh();
	house->generateObjectBufferMesh("../models/house.dae");
	test = new TextureScreen();
	test->generateObjectBuffer();

	// load tecture
	grass = new Texture(GL_TEXTURE_2D, "../textures/grass.JPG");
	grass->Load();

	white = new Texture(GL_TEXTURE_2D, "../textures/house.png");
	white->Load();

	red = new Texture(GL_TEXTURE_2D, "../textures/red.JPG");
	red->Load();

	sight = new Texture(GL_TEXTURE_2D, "../textures/test.png");
	sight->Load();
	
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	if (key == 'w') {
		transform_y = transform_y - 0.1f * cos(rotate_x*PI / 180.0f);
		transform_x = transform_x + 0.1f * sin(rotate_x*PI / 180.0f);
		if (collision()) {
			transform_y = transform_y + 0.1f * cos(rotate_x*PI / 180.0f);
			transform_x = transform_x - 0.1f * sin(rotate_x*PI / 180.0f);
		}

		printf("Transform: x  %f  y %f\n", transform_x, transform_y);
	}
	if (key == 'a') {
		rotate_x = fmod(rotate_x - 1.0f, 360.0);
	}
	if (key == 's') {
		transform_y = transform_y + 0.1f * cos(rotate_x*PI / 180.0f);
		transform_x = transform_x - 0.1f * sin(rotate_x*PI / 180.0f);
		if (collision()) {
			transform_y = transform_y - 0.1f * cos(rotate_x*PI / 180.0f);
			transform_x = transform_x + 0.1f * sin(rotate_x*PI / 180.0f);
		}
		printf("Transform: x  %f  y %f\n", transform_x, transform_y);
	}
	if (key == 'd') {
		rotate_x = fmod(rotate_x + 1.0f, 360.0);
	}
	if (key == ' ') {
		
		if (viewstate == 0) {
			viewstate = 1;
		} else if (viewstate == 1){
			viewstate = 0;
		}
	}

	if (key == 'e') {
		if (shoot()) {
			enemystate = 1;
		}

		if (shoot2()) {
			enemystate2 = 1;
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
