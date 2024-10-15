#include "includes/Angel.h"
#include <stdlib.h>

using namespace Angel;

typedef vec2 point2;


const int n_points = 5000;

void init() {
	point2 points[n_points];

	point2 vertices[3] = { point2(-1.0, 0), point2(1, 0), point2(0,1) };

	points[0] = point2(0.5, 0);

	for(int i =1 ;i<n_points; i++)
	{
		int j = rand()%3;
		points[i] = (points[i-1] + vertices[j])/2;
	}

	GLuint abuffer;
	glGenVertexArrays(1, &abuffer);
	glBindVertexArray(abuffer);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
}

void displayFunc() {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_POINTS, 0, n_points);
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Sierpinski Gasket");
	glewInit();
	glutDisplayFunc(displayFunc);
	init();

	GLuint program = InitShader("vsource.glsl", "fsource.glsl");
	GLuint loc =glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	glutMainLoop();
}

