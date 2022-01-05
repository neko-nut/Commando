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
#include "Board.h"
#include "TextureScreen.h"
#include "Texture.h"
#include "SkyBox.h"


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
Shader* boardShader;
Shader* skyboxShader;

Mesh* humanMesh;
Mesh* groundMesh;
Mesh* houseMesh;
Aim* aim;
Board* board;
SkyBox* skybox;

Texture* grassTexture;
Texture* houseTexture;
Texture* red;
Texture* gameover;
Texture* win;

TextureScreen* human;

GLfloat transform_x = 0.0f;
GLfloat transform_y = 0.0f;
float rotate_x = 0.0f;

int movemouse = 0;
GLfloat view_rotate_x = 0.0f;
GLfloat view_rotate_y = 0.0f;
GLfloat mouse_x, mouse_y;

int const numberOfEnemies = 6;
float enemyLocation[numberOfEnemies][2] = {
	{0.0f, 50.0f},
	{-30.0f, 20.0f},
	{60.0f, 50.0f},
	{10.0f, 15.0f},
	{-40.0f, 0.0f},
	{30.0f, -50.0f},
};

int enemyStates[numberOfEnemies] = { 0 };


int viewstate = 0;
int enemystate = 0;
int enemystate2 = 0;
int gamestate = 0;

GLfloat greenColor[3] = {0.0f, 0.55f, 0.0f};
GLfloat bodyColor[3] = { 0.9f, 0.88f, 0.62f };
GLfloat oramgeColor[3] = { 1.0f, 0.5f, 0.0f };
GLfloat whiteColor[3] = { 1.0f, 1.0f, 1.0f };
GLfloat redColor[3] = { 1.0f, 0.0f, 0.0f };
float walk = 0;

float houseLocation[5][2] = {
	{0.0f, 30.0f},
	{20.0f, 60.0f},
	{-5.0f, -50.0f},
	{70.0f, 0.0f},
	{-45.0f, 20.0f},
};

float targetLocation[2] = { -50.0f, 0 };


float houseBox[2] = {11.0f, 10.0f};
float enemyBox[2] = { 1.0f, 1.0f };


void shoot();
void moveEnemies();
bool collision_y(float x, float y);
bool collision_x(float x, float y);
void collision_enemy();


void drawObjects(mat4 view, mat4 persp_proj, mat4 model) {
	glEnable(GL_DEPTH_TEST);
	glUseProgram(cubeShader->ID);
	//Declare your uniform variables that will be used in your shader
	int cube_matrix_location = glGetUniformLocation(cubeShader->ID, "model");
	int cube_view_mat_location = glGetUniformLocation(cubeShader->ID, "view");
	int cube_proj_mat_location = glGetUniformLocation(cubeShader->ID, "proj");
	int color = glGetUniformLocation(cubeShader->ID, "ourColor");

	mat4 humanModel = identity_mat4();
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
	leftShoulderModel = rotate_x_deg(leftShoulderModel, 20 * sin(1.5f * walk));
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
	rightShoulderModel = rotate_x_deg(rightShoulderModel, -20 * sin(1.5f * walk));
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
	leftBottomModel = rotate_x_deg(leftBottomModel, -20 * sin(1.5f * walk));
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
	rightBottomModel = rotate_x_deg(rightBottomModel, 20 * sin(1.5f * walk));
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


	for (int i = 0; i < 5; i++) {
		mat4 modelBarrier = identity_mat4();
		modelBarrier = scale(modelBarrier, vec3(0.01f, 0.1f, 0.01f));
		modelBarrier = scale(modelBarrier, vec3(1.8f, 2.0f, 1.4f));
		modelBarrier = translate(modelBarrier, vec3(0.01 *houseLocation[i][0], 1.0f, 0.01 * houseLocation[i][1]));

		modelBarrier = modelGround * modelBarrier;

		// Update the appropriate uniform and draw the mesh again
		glUniformMatrix4fv(texture_matrix_location, 1, GL_FALSE, modelBarrier.m);
		houseTexture->Bind(GL_TEXTURE0);
		houseMesh->linkCurrentBuffertoShader(textureShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, houseMesh->mesh_data.mPointCount);
	}




	glUseProgram(meshShader->ID);
	//Declare your uniform variables that will be used in your shader
	int mesh_matrix_location = glGetUniformLocation(meshShader->ID, "model");
	int mesh_view_mat_location = glGetUniformLocation(meshShader->ID, "view");
	int mesh_proj_mat_location = glGetUniformLocation(meshShader->ID, "proj");
	int enemyColor = glGetUniformLocation(meshShader->ID, "ourColor");

	glUniformMatrix4fv(mesh_proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(mesh_view_mat_location, 1, GL_FALSE, view.m);
	

	for (int i = 0; i < numberOfEnemies; i++) {
		if (enemyStates[i] != 1) {
			if (enemyStates[i] == 2 && viewstate == 1) {
				glUniform3fv(enemyColor, 1, redColor);
			} else {
				glUniform3fv(enemyColor, 1, oramgeColor);
			}
			mat4 modelEnemies = identity_mat4();
			modelEnemies = scale(modelEnemies, vec3(0.01f, 0.1f, 0.01f));
			modelEnemies = scale(modelEnemies, vec3(0.25f, 0.25f, 0.25f));
			modelEnemies = translate(modelEnemies, vec3(0.01 * enemyLocation[i][0], 0.0f, 0.01 * enemyLocation[i][1]));
			modelEnemies = translate(modelEnemies, vec3(0.0f, 0.935f, 0.0f));
			modelEnemies = modelGround * modelEnemies;
			glUniformMatrix4fv(mesh_matrix_location, 1, GL_FALSE, modelEnemies.m);
			humanMesh->linkCurrentBuffertoShader(meshShader->ID);
			glDrawArrays(GL_TRIANGLES, 0, humanMesh->mesh_data.mPointCount);
		}
	}

	glUseProgram(cubeShader->ID);

	mat4 targetModel = identity_mat4();
	targetModel = scale(targetModel, vec3(0.01f, 0.1f, 0.01f));
	targetModel = translate(targetModel, vec3(0.01f * targetLocation[0], 1.05f, 0.01f * targetLocation[1]));
	targetModel = modelGround * targetModel;

	glUniformMatrix4fv(cube_proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(cube_view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(cube_matrix_location, 1, GL_FALSE, targetModel.m);
	glUniform3fv(color, 1, whiteColor);
	glBindBuffer(GL_ARRAY_BUFFER, human->VBO);
	human->linkCurrentBuffertoShader(cubeShader->ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisable(GL_DEPTH_TEST);

}

void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.53f, 0.81f, 0.92f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, width, height);
	// Root of the Hierarchy
	mat4 view = identity_mat4();
	view = rotate_y_deg(view, view_rotate_x);
	view = rotate_x_deg(view, view_rotate_y);

	view = rotate_y_deg(view, 180.0f);
	view = rotate_x_deg(view, 10.0f);
	view = translate(view, vec3(0.0, 0.0, -5.0f));
	
	if (viewstate == 1) {
		view = translate(view, vec3(0.0, 0.3f, 5.3f));
		view = rotate_x_deg(view, -10.0f);
	}

	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	mat4 model = identity_mat4();
	model = rotate_y_deg(model, rotate_x);
	mat4 skybox_model = model;
	model = translate(model, vec3(0.0f, -10.5f, 0.0f));
	
	
	glDepthMask(GL_FALSE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(skyboxShader->ID);
	int skybox_view_mat_location = glGetUniformLocation(skyboxShader->ID, "view");
	int skybox_proj_mat_location = glGetUniformLocation(skyboxShader->ID, "projection");
	int skybox_model_mat_location = glGetUniformLocation(skyboxShader->ID, "model");
	skybox_model = scale(skybox_model, vec3(10000.0f, 10000.0f, 10000.0f));
	glUniformMatrix4fv(skybox_proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(skybox_view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(skybox_model_mat_location, 1, GL_FALSE, skybox_model.m);
	// skybox cube
	skybox->linkCurrentBuffertoShader(skyboxShader->ID);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	

	drawObjects(view, persp_proj, model);

	if (gamestate == 1) {
		glUseProgram(boardShader->ID);
		glBindBuffer(GL_ARRAY_BUFFER, board->VBO);
		gameover->Bind(GL_TEXTURE0);
		board->linkCurrentBuffertoShader(boardShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 3, 3);
	}
	else if (gamestate == 2) {
		glUseProgram(boardShader->ID);
		glBindBuffer(GL_ARRAY_BUFFER, board->VBO);
		win->Bind(GL_TEXTURE0);
		board->linkCurrentBuffertoShader(boardShader->ID);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 3, 3);
	}
	if (viewstate == 1) {
		glUseProgram(aimShader->ID);
		glBindBuffer(GL_ARRAY_BUFFER, aim->VBO);
		aim->linkCurrentBuffertoShader(aimShader->ID);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	}

	if (viewstate == 1) {
		view = rotate_x_deg(view, 10.0f);
		view = translate(view, vec3(0.0, -0.3f, -5.3f));
	}
	view = translate(view, vec3(0.0, 0.0, 5.0f));
	view = rotate_x_deg(view, -10.0f);
	view = rotate_y_deg(view, -180.0f);
	view = rotate_x_deg(view, -view_rotate_y);
	view = rotate_y_deg(view, -view_rotate_x);

	view = rotate_y_deg(view, 180.0f);
	view = rotate_x_deg(view, 10.0f);
	view = translate(view, vec3(0.0, 0.0, -5.0f));
	
	view = rotate_x_deg(view, 80.0f);
	view = translate(view, vec3(0.0, 0.0, -15.0f));
	view = translate(view, vec3(0.0, -5.0, 0.0f));

	//view = translate(view, vec3(0.0, 0.0, -15.0f));
	//model = translate(view, vec3(1.0, 0.0, 0.0f));

	glViewport(0, height * 0.8, width * 0.2, height * 0.2);
	drawObjects(view, persp_proj, model);


	if (gamestate == 0) {
		shoot();
		moveEnemies();
		collision_enemy();
	}
	glutSwapBuffers();

}


void moveEnemies() {
	for (int i = 0; i < numberOfEnemies; i++) {
		float x = -enemyLocation[i][0] - transform_x;
		float y = enemyLocation[i][1] + transform_y;
		float len = sqrt(x*x + y*y);

		enemyLocation[i][0] = enemyLocation[i][0] + (x / len) * 0.01f;
		if (collision_x(-enemyLocation[i][0], -enemyLocation[i][1])) {
			enemyLocation[i][0] = enemyLocation[i][0] - (x / len) * 0.01f;
		}

		enemyLocation[i][1] = enemyLocation[i][1] - (y / len) * 0.01f;
		if (collision_y(-enemyLocation[i][0], -enemyLocation[i][1])) {
			enemyLocation[i][1] = enemyLocation[i][1] + (y / len) * 0.01f;
		}
	}
}



bool cansee(float enemy_x, float enemy_y, float angle) {
	float k = tan(angle);
	float y = -transform_y;
	float x = transform_x;
	float b = y - k * x;


	float c = cos(angle);
	float s = sin(angle);
	float xe = -enemy_x - transform_x;
	float ye = enemy_y + transform_y;


	//printf("cansee: %f  %f  %f  %f \n", c, s, x1, y1);
	if (c*xe > -0.01 && s*ye > -0.01) {
		for (int i = 0; i < 5; i++) {
			float x1 = -houseLocation[i][0] + houseBox[0];
			float x2 = -houseLocation[i][0] - houseBox[0];
			float y1 = x1 * k + b;
			float y2 = x2 * k + b;
			float y3 = houseLocation[i][1] + houseBox[1];
			float y4 = houseLocation[i][1] - houseBox[1];
			float x3 = (y3 - b) / k;
			float x4 = (y4 - b) / k;
			//printf("%i: %f, %f, %f, %f \n", i, y1, y2, y3, y4);
			if (k > 100) {
				if (x < max(x1, x2) && x > min(x1, x2)) {
					float e_distance = xe * xe + ye * ye;
					float xh = -houseLocation[i][0] - transform_x;
					float yh = houseLocation[i][1] + transform_y;

					float h_distance = xh * xh + yh * yh;
					if (e_distance > h_distance) {
						return false;
					}
				}
			}
			else {
				if ((y1 > min(y3, y4) && y1 < max(y3, y4)) ||
					(y2 > min(y3, y4) && y2 < max(y3, y4)) ||
					(x4 > min(x1, x2) && x4 < max(x1, x2)) ||
					(x3 > min(x1, x2) && x3 < max(x1, x2))) {
					float e_distance = xe * xe + ye * ye;
					float xh = -houseLocation[i][0] - transform_x;
					float yh = houseLocation[i][1] + transform_y;

					float h_distance = xh * xh + yh * yh;
					if (e_distance > h_distance) {
						return false;
					}
				}
			}
		}
		return true;
	}

	return false;

}




void shoot() {
	float angle = (90 - rotate_x) * PI / 180.0f;
	// y = k *x + b
	float k = tan(angle);
	float y = -transform_y;
	float x = transform_x;
	float b = y - k * x;

	for (int i = 0; i < numberOfEnemies; i++) {
		if (enemyStates[i] != 1) {
			// enemy can be seen
			float x1 = -enemyLocation[i][0] + enemyBox[0];
			float x2 = -enemyLocation[i][0] - enemyBox[0];
			float y1 = x1 * k + b;
			float y2 = x2 * k + b;
			float y3 = enemyLocation[i][1] + enemyBox[1];
			float y4 = enemyLocation[i][1] - enemyBox[1];
			float x3 = (y3 - b) / k;
			float x4 = (y4 - b) / k;
			if (enemyStates[i] == 2) {
				enemyStates[i] = 0;
			}
			//printf("%i: %f, %f, %f, %f \n", i, y1, y2, y3, y4);
			if (k > 100) {
				if (x < max(x1, x2) && x > min(x1, x2)) {
					if (cansee(enemyLocation[i][0], enemyLocation[i][1], angle)) {
						//printf("success %i\n", i);
						enemyStates[i] = 2;
					}

				}
			}
			else {
				if ((y1 > min(y3, y4) && y1 < max(y3, y4)) ||
					(y2 > min(y3, y4) && y2 < max(y3, y4)) ||
					(x4 > min(x1, x2) && x4 < max(x1, x2)) ||
					(x3 > min(x1, x2) && x3 < max(x1, x2))) {
					if (cansee(enemyLocation[i][0], enemyLocation[i][1], angle)) {
						//printf("success %i\n", i);
						enemyStates[i] = 2;
					}
				}
			}
		}
	}
}


bool collision_y(float x, float y) {
	if (y > 70 || y < -70) {
		return true;
	}
	for (int i = 0; i < 5; i++) {
		if (y < -houseLocation[i][1] + houseBox[1] 
			&& x < -houseLocation[i][0] + houseBox[0] 
			&& x > -houseLocation[i][0] -houseBox[0]
			&& y > -houseLocation[i][1] - houseBox[1])
		{
			if (y < -houseLocation[i][1] + houseBox[1] && y > -houseLocation[i][1] - houseBox[1]) {
				return true;
			}
		}
	}
	return false;
}

bool collision_x(float x, float y) {
	if (x > 70 || x < -70) {
		return true;
	}
	for (int i = 0; i < 5; i++) {
		if (y < -houseLocation[i][1] + houseBox[1]
			&& x < -houseLocation[i][0] + houseBox[0]
			&& x > -houseLocation[i][0] - houseBox[0]
			&& y > -houseLocation[i][1] - houseBox[1])
		{
			if (x < -houseLocation[i][0] + houseBox[0] && x > -houseLocation[i][0] - houseBox[0]) {
				return true;
			}
		}
	}
	return false;
}

void collision_enemy() {
	for (int i = 0; i < numberOfEnemies; i++) {
		if (enemyStates[i] != 1) {
			if (transform_y < -enemyLocation[i][1] + enemyBox[1]
				&& transform_x < -enemyLocation[i][0] + enemyBox[0]
				&& transform_x > -enemyLocation[i][0] - enemyBox[0]
				&& transform_y > -enemyLocation[i][1] - enemyBox[1])
			{
				gamestate = 1;
			}
		}
	}
}


void collision_target() {
	for (int i = 0; i < numberOfEnemies; i++) {
		if (transform_y < -targetLocation[1] + 1
			&& transform_x < -targetLocation[0] + 1
			&& transform_x > -targetLocation[0] - 1
			&& transform_y > -targetLocation[1] - 1)
		{
			gamestate = 2;
		}
	}
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
	aimShader = new Shader();
	aimShader->CompileShaders("../shades/aimVertexShader.txt", "../shades/aimFragmentShader.txt");
	boardShader = new Shader();
	boardShader->CompileShaders("../shades/boardVertexShader.txt", "../shades/boardFragmentShader.txt");
	textureShader = new Shader();
	textureShader ->CompileShaders("../shades/textureVertexShader.txt", "../shades/textureFragmentShader.txt");
	cubeShader = new Shader();
	cubeShader->CompileShaders("../shades/cubeVertexShader.txt", "../shades/cubeFragmentShader.txt");
	skyboxShader = new Shader();
	skyboxShader->CompileShaders("../shades/skyboxVertexShader.txt", "../shades/skyboxFragmentShader.txt");
	meshShader = new Shader();
	meshShader->CompileShaders("../shades/meshVertexShader.txt", "../shades/meshFragmentShader.txt");
	
	
	// load mesh into a vertex buffer array
	humanMesh = new Mesh();
	humanMesh->generateObjectBufferMesh("../models/human.dae");
	aim = new Aim();
	aim->generateObjectBuffer();
	board = new Board();
	board->generateObjectBuffer();
	groundMesh = new Mesh();
	groundMesh->generateObjectBufferMesh("../models/ground.dae");
	houseMesh = new Mesh();
	houseMesh->generateObjectBufferMesh("../models/house.dae");
	human = new TextureScreen();
	human->generateObjectBuffer();
	skybox = new SkyBox();
	skybox->generateObjectBuffer();


	// load tecture
	grassTexture = new Texture(GL_TEXTURE_2D, "../textures/grass.JPG");
	grassTexture->Load();
	houseTexture = new Texture(GL_TEXTURE_2D, "../textures/house.png");
	houseTexture->Load();
	red = new Texture(GL_TEXTURE_2D, "../textures/red.JPG");
	red->Load();
	gameover = new Texture(GL_TEXTURE_2D, "../textures/gameover.png");
	gameover->Load();
	win = new Texture(GL_TEXTURE_2D, "../textures/win.png");
	win->Load();

	
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	if (gamestate == 0) {
		if (key == 'w' && viewstate == 0) {
			transform_y = transform_y - 0.1f * cos(rotate_x*PI / 180.0f);
			if (collision_y(transform_x, transform_y)) {
				transform_y = transform_y + 0.1f * cos(rotate_x*PI / 180.0f);
			}
			transform_x = transform_x + 0.1f * sin(rotate_x*PI / 180.0f);
			if (collision_x(transform_x, transform_y)) {
				transform_x = transform_x - 0.1f * sin(rotate_x*PI / 180.0f);
			}
			collision_target();
			walk = walk + 0.1f;
			printf("Transform: x  %f  y %f\n", transform_x, transform_y);
		}
		if (key == 'a') {
			rotate_x = fmod(rotate_x - 1.0f, 360.0);
			printf("Rotate: %f\n", rotate_x);
		}
		if (key == 's' && viewstate == 0) {
			transform_y = transform_y + 0.1f * cos(rotate_x*PI / 180.0f);
			if (collision_y(transform_x, transform_y)) {
				transform_y = transform_y - 0.1f * cos(rotate_x*PI / 180.0f);
			}
			transform_x = transform_x - 0.1f * sin(rotate_x*PI / 180.0f);
			if (collision_x(transform_x, transform_y)) {
				transform_x = transform_x + 0.1f * sin(rotate_x*PI / 180.0f);
			}
			collision_target();
			walk = walk - 0.1f;
			printf("Transform: x  %f  y %f\n", transform_x, transform_y);
		}
		if (key == 'd') {
			rotate_x = fmod(rotate_x + 1.0f, 360.0);
			printf("Rotate: %f\n", rotate_x);
		}
		if (key == ' ') {
			movemouse = 0;
			view_rotate_x = 0;
			view_rotate_y = 0;
			if (viewstate == 0) {
				viewstate = 1;
			}
			else if (viewstate == 1) {
				viewstate = 0;
			}
		}
	}
	
	glutPostRedisplay();
}


void mousepress(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (gamestate == 0) {
			if (viewstate == 1) {
				for (int i = 0; i < 10; i++) {
					if (enemyStates[i] == 2) {
						enemyStates[i] = 1;
					}
				}
			}
		}else if (gamestate == 1 || gamestate == 2) {
			gamestate = 0;
			transform_x = 0.0f;
			transform_y = 0.0f;
			rotate_x = 0.0f;
			walk = 0;
			enemyLocation[0][0] = 0.0f;
			enemyLocation[0][1] = 50.0f;
			enemyLocation[1][0] = -30.0f;
			enemyLocation[1][1] = 20.0f;
			enemyLocation[2][0] = 60.0f;
			enemyLocation[2][1] = 50.0f;
			enemyLocation[3][0] = 10.0f;
			enemyLocation[3][1] = 15.0f;
			enemyLocation[4][0] = -40.0f;
			enemyLocation[4][1] = 0.0f;
			enemyLocation[5][0] = 30.0f;
			enemyLocation[5][1] = -50.0f;

			for (int i = 0; i < 6; i++) {
				enemyStates[i] = 0;
			}
			
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (viewstate == 0) {
			if (movemouse == 0) {
				mouse_x = x;
				mouse_y = y;
				movemouse = 1;
			}
			else {
				movemouse = 0;
				view_rotate_x = 0;
				view_rotate_y = 0;
			}
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
