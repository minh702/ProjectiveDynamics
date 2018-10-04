#pragma once

#include <string>
#include <fstream>
#include < iostream>
#include <vector>
#include<glut.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm\gtx\rotate_vector.hpp"
#include "RenderObject.h"

using namespace std;
using namespace glm;

char s[100];
int w1, h1;
bool WireFrame = 1, Dots = 0, Mesh = 0;

int frame = 0, time, timebase = 0;

float px = 0;
float py = 5;
float pz = 0;
void setOrthographicProjection()
{
	// 행렬을 투영모드로 변경합니다.
	glMatrixMode(GL_PROJECTION);
	// 원근투영에 대한 환경변수를 
	// 가지고 있는 이전 행렬을 저장합니다.
	glPushMatrix();
	// 행렬을 초기화합니다.
	glLoadIdentity();
	// 2D 직교투영을 설정합니다.
	gluOrtho2D(0, w1, 0, h1);
	// y 축을 뒤집습니다.
	glScalef(1, -1, 1);
	// 좌측아래의 원점을 
	// 좌측위로 옮깁니다.
	glTranslatef(0, -h1, 0);
	glMatrixMode(GL_MODELVIEW);
}

void init(GLvoid)
{
	//_textureId = loadTexture(image);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightPos[4] = { -1.0,1.0,0.5,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&lightPos);

	glEnable(GL_LIGHT1);

	GLfloat lightAmbient1[4] = { 0.0,0.0,0.0,0.0 };
	GLfloat lightPos1[4] = { 1.0,0.0,-0.2,0.0 };
	GLfloat lightDiffuse1[4] = { 0.5,0.5,0.3,0.0 };

	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *)&lightPos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *)&lightAmbient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *)&lightDiffuse1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void renderBitmapString(float x, float y, void *font, char *string)
{
	char *c;
	int x1 = x;
	for (c = string; *c != '\0'; c++)
	{
		glRasterPos2f(x1, y);
		glutBitmapCharacter(font, *c);
		x1 = x1 + glutBitmapWidth(font, *c);
	}
}

void resetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void renderBitmapString(float x, float y, float z, void *font, char *string)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

bool showBV = false;

vec3 camPos(0,0,30);
vec3 dir(0, 0, -1);
vec3 side(1, 0, 0);
vec3 up(0, 1, 0);

bool start = false;


float speed = 0.5f;
float angleUp = 4.0f;

int curX, curY;

int prevX, prevY;

vector<RenderObject> ros;


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;


	w1 = w;
	h1 = h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 1, 1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}



int dx;
int dy;



void mouseMove(int x, int y)
{

	curX = x;
	curY = y;


	dx = curX - prevX;
	dy = curY - prevY;

	if (start == true)
	{
		dir = rotate(dir, radians((float)dx/2),up);
		side = rotate(side, radians((float)dx/2), up);

		dir = rotate(dir, radians((float)dy / 2), side);
		up = rotate(up, radians((float)dy / 2), side);

	}

	prevX = curX;
	prevY = curY;
}

void mouseButton(int button, int state, int x, int y) {

	

	
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP)
			start = false;
		else
		{
			prevX = x;
			prevY = y;
			start = true;
		}
	}

}


void renderScene(void) {

	glClearColor(20.f/255, 180.f/255, 255.f/255,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable(GL_DEPTH_TEST);

	// Reset transformations

	glLoadIdentity();
	// Set the camera
	gluLookAt(camPos.x, camPos.y, camPos.z,
		camPos.x + 10 * dir.x, camPos.y + 10 * dir.y, camPos.z + 10 * dir.z,
		0.0f, 1.0f, 0.0f);

	//gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	

	
	for (auto & ro : ros)
	{
		ro.stepSimulation(1 / 60.f);
		ro.DrawWireFrame();
	}



	glutSwapBuffers();
	glutPostRedisplay();
}



void processSpecialKeys(int key, int xx, int yy)
{

}

void processsNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);

	case 'w':
	
		camPos +=speed*dir;
		break;

	case 's':
	
		camPos -= speed*dir;
	
		break;

	case 'd':
		camPos += speed*side;
		break;

	case 'a':

		camPos -= speed*side;
		break;




	case 'v' :

		for (auto & ro : ros)
		{
			ro.stepSimulation(1 / 24.f);
		}
		break;



	}

}