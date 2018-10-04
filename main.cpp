

#define _CRT_SECURE_NO_WARNINGS  
#include<glut.h>


#include "RenderObject.h"
#include "glutUtilFuctions.h"
int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	init();
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(840, 650);
	glutCreateWindow("Flag Simulation Demo");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(processsNormalKeys);
	glutIdleFunc(renderScene);
	// enter GLUT event processing loop

	

	RenderObject building("plane3.obj");
	ros.push_back(building);


	
	glutMainLoop();

	return 0;
}