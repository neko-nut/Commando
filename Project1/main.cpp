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
#include "Aim.h"
#include "TextureScreen.h"
#include "Texture.h"
#include "Game.h"


#define  PI 3.1415926

using namespace std;

int width = 800;
int height = 600;

GLfloat rotate_y = 0.0f;
Shader* meshShader;
Shader* aimShader;
Shader* cubeShader;
Shader* textureShader;
Shader* testShader;
Mesh *humanMesh;
Mesh *groundMesh;
Mesh *houseMesh;
Aim *aim;

Texture* grassTexture;
Texture* houseTexture;
Texture* red;
Texture* sight;

Game* game;
TextureScreen* human;

GLfloat transform_x = 0.0f;
GLfloat transform_y = 0.0f;
float rotate_x = 0.0f;

int movemouse = 0;
GLfloat view_rotate_x = 0.0f;
GLfloat view_rotate_y = 0.0f;
GLfloat mouse_x, mouse_y;

GLfloat enemy_x = 10.0f;
GLfloat enemy_y = 20.0f;


GLfloat enemy_x2 = -30.0f;
GLfloat enemy_y2 = 20.0f;

int viewstate = 0;
int enemystate = 0;
int enemystate2 = 0;

GLfloat greenColor[3] = {0.0f, 0.55f, 0.0f};
GLfloat bodyColor[3] = { 0.9f, 0.88f, 0.62f };
float walk = 0;




void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.53f, 0.81f, 0.92f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (viewstate == 1) {
		glUseProgram(aimShader->ID);
		glBindBuffer(GL_ARRAY_BUFFER, aim->VBO);
		aim->linkCurrentBuffertoShader(aimShader->ID);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	}


	// Root of the Hierarchy
	mat4 view = identity_mat4();
	view = rotate_y_deg(view, view_rotate_x);
	view = rotate_x_deg(view, view_rotate_y);
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
	


	glUseProgram(cubeShader->ID);
	//Declare your uniform variables that will be used in your shader
	int cube_matrix_location = glGetUniformLocation(cubeShader->ID, "model");
	int cube_view_mat_location = glGetUniformLocation(cubeShader->ID, "view");
	int cube_proj_mat_location = glGetUniformLocation(cubeShader->ID, "proj");
	int color = glGetUniformLocation(cubeShader->ID, "ourColor");

	mat4 humanModel = identity_mat4();
	humanModel = translate(humanModel, vec3(0.0f, 0.0f, -3.0f));
	humanModel = rotate_y_deg(humanModel, -rotate_x);
	humanModel = translate(humanModel, vec3(0.0f, 8.0f, 0.0f));
	humanModel = scale(humanModel, vec3(1.1f, 1.2f, 1.1f));
	humanModel = model * humanModel;

	mat4 bodyModel = identity_mat4();
	bodyModel = scale(bodyModel, vec3(0.6f, 0.85f, 0.35f));
	bodyModel = humanModel * bodyModel;

	glUniformMatrix4fv(cube_proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(cube_view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, bodyModel.m);
	glUniform3fv(color, 1, greenColor);

	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	mat4 headModel = identity_mat4();
	headModel = translate(headModel, vec3(0.0f, 1.2f, 0.0f));
	headModel = scale(headModel, vec3(0.6f, 0.6f, 0.6f));
	headModel = humanModel * headModel;

	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, headModel.m);
	glUniform3fv(color, 1, bodyColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	mat4 leftShoulderModel = identity_mat4();
	leftShoulderModel = rotate_x_deg(leftShoulderModel, 20 * sin(walk));
	leftShoulderModel = translate(leftShoulderModel, vec3(1.5f, 3.5f, 0.0f));
	leftShoulderModel = scale(leftShoulderModel, vec3(0.3f, 0.1f, 0.3f));
	leftShoulderModel = humanModel * leftShoulderModel;

	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, leftShoulderModel.m);
	glUniform3fv(color, 1, greenColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);




	mat4 leftArmModel = identity_mat4();
	leftArmModel = translate(leftArmModel, vec3(0.0f, -0.5f, 0.0f));
	leftArmModel = scale(leftArmModel, vec3(1.0f, 5.5f, 1.0f));
	leftArmModel = leftShoulderModel * leftArmModel;

	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, leftArmModel.m);
	glUniform3fv(color, 1, bodyColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);




	mat4 rightShoulderModel = identity_mat4();
	rightShoulderModel = rotate_x_deg(rightShoulderModel, -20 * sin(walk));
	rightShoulderModel = translate(rightShoulderModel, vec3(-1.5f, 3.5f, 0.0f));
	rightShoulderModel = scale(rightShoulderModel, vec3(0.3f, 0.1f, 0.3f));
	rightShoulderModel = humanModel * rightShoulderModel;

	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, rightShoulderModel.m);
	glUniform3fv(color, 1, greenColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	mat4 rightArmModel = identity_mat4();
	rightArmModel = translate(rightArmModel, vec3(0.0f, -0.5f, 0.0f));
	rightArmModel = scale(rightArmModel, vec3(1.0f, 5.5f, 1.0f));
	rightArmModel = rightShoulderModel * rightArmModel;

	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, rightArmModel.m);
	glUniform3fv(color, 1, bodyColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);



	mat4 leftBottomModel = identity_mat4();
	leftBottomModel = rotate_x_deg(leftBottomModel, -20 * sin(walk));
	leftBottomModel = translate(leftBottomModel, vec3(0.5f, -2.5f, 0.0f));
	leftBottomModel = scale(leftBottomModel, vec3(0.3f, 0.2f, 0.3f));
	leftBottomModel = humanModel * leftBottomModel;

	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, leftBottomModel.m);
	glUniform3fv(color, 1, greenColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	mat4 leftLegModel = identity_mat4();
	leftLegModel = translate(leftLegModel, vec3(0.0f, -0.5f, 0.0f));
	leftLegModel = scale(leftLegModel, vec3(1.0f, 3.5f, 1.0f));
	leftLegModel = leftBottomModel * leftLegModel;

	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, leftLegModel.m);
	glUniform3fv(color, 1, bodyColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);



	mat4 rightBottomModel = identity_mat4();
	rightBottomModel = rotate_x_deg(rightBottomModel, 20 * sin(walk));
	rightBottomModel = translate(rightBottomModel, vec3(-0.5f, -2.5f, 0.0f));
	rightBottomModel = scale(rightBottomModel, vec3(0.3f, 0.2f, 0.3f));
	rightBottomModel = humanModel * rightBottomModel;

	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, rightBottomModel.m);
	glUniform3fv(color, 1, greenColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	mat4 rightLegModel = identity_mat4();
	rightLegModel = translate(rightLegModel, vec3(0.0f, -0.5f, 0.0f));
	rightLegModel = scale(rightLegModel, vec3(1.0f, 3.5f, 1.0f));
	rightLegModel = rightBottomModel * rightLegModel;

	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, rightLegModel.m);
	glUniform3fv(color, 1, bodyColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);




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
	groundMesh->linkCurrentBuffertoShader(textureShader->ID);
	grassTexture->Bind(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, groundMesh->mesh_data.mPointCount);
	
	
	mat4 modelBarrier = identity_mat4();
	modelBarrier = scale(modelBarrier, vec3(0.01f, 0.1f, 0.01f));
	modelBarrier = scale(modelBarrier, vec3(2.0f, 2.0f, 2.0f));
	modelBarrier = translate(modelBarrier, vec3(0.0f, 1.0f, 0.3f));

	modelBarrier = modelGround * modelBarrier;

	// Update the appropriate uniform and draw the mesh again
	glUniformMatrix4fv(texture_matrix_location, 1, GL_FALSE, modelBarrier.m);
	houseTexture->Bind(GL_TEXTURE0);
	houseMesh->linkCurrentBuffertoShader(textureShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, houseMesh->mesh_data.mPointCount);


	//enemy_y = enemy_y - 0.01;
	mat4 modelEnemies = identity_mat4();
	modelEnemies = translate(modelEnemies, vec3(enemy_x, 0.0f, enemy_y));
	modelEnemies = scale(modelEnemies, vec3(0.01f, 0.1f, 0.01f));
	modelEnemies = scale(modelEnemies, vec3(0.25f, 0.25f, 0.25f));
	modelEnemies = translate(modelEnemies, vec3(0.0f, 0.935f, 0.0f));
	modelEnemies = modelGround * modelEnemies;
	glUniformMatrix4fv(texture_matrix_location, 1, GL_FALSE, modelEnemies.m);
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
	glUniformMatrix4fv(texture_matrix_location, 1, GL_FALSE, modelEnemies.m);
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
	aimShader = new Shader();
	aimShader->CompileShaders("../shades/aimVertexShader.txt", "../shades/aimFragmentShader.txt");
	textureShader = new Shader();
	textureShader ->CompileShaders("../shades/textureVertexShader.txt", "../shades/textureFragmentShader.txt");

	testShader = new Shader();
	testShader->CompileShaders("../shades/texturecubeVertexShader.txt", "../shades/texturecubeFragmentShader.txt");
	cubeShader = new Shader();
	cubeShader->CompileShaders("../shades/cubeVertexShader.txt", "../shades/cubeFragmentShader.txt");
	
	// load mesh into a vertex buffer array
	humanMesh = new Mesh();
	humanMesh->generateObjectBufferMesh("../models/human.dae");
	aim = new Aim();
	aim->generateObjectBuffer();
	groundMesh = new Mesh();
	groundMesh->generateObjectBufferMesh("../models/ground.dae");
	houseMesh = new Mesh();
	houseMesh->generateObjectBufferMesh("../models/house.dae");
	human = new TextureScreen();
	human->generateObjectBuffer();

	// load tecture
	grassTexture = new Texture(GL_TEXTURE_2D, "../textures/grass.JPG");
	grassTexture->Load();

	houseTexture = new Texture(GL_TEXTURE_2D, "../textures/house.png");
	houseTexture->Load();

	red = new Texture(GL_TEXTURE_2D, "../textures/red.JPG");
	red->Load();

	
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
		walk = walk + 0.1f;
		printf("walk: %f \n", sin(walk));
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


	glutPostRedisplay();
}


void mousepress(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (viewstate == 1) {
			if (shoot()) {
				enemystate = 1;
			}
			if (shoot2()) {
				enemystate2 = 1;
			}
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (movemouse == 0) {
			mouse_x = x;
			mouse_y = y;
			movemouse = 1;
		} else {
			movemouse = 0;
			view_rotate_x = 0;
			view_rotate_y = 0;
		}
		
	}

	glutPostRedisplay();
}


void mousemove(int x, int y) {
	if (movemouse == 1) {
		view_rotate_x += (x - mouse_x) * 0.1f;
		view_rotate_y += (y - mouse_y) * 0.1f;
		mouse_x = x;
		mouse_y = y;
	}
	
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
	glutMouseFunc(mousepress);
	glutPassiveMotionFunc(mousemove);

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
