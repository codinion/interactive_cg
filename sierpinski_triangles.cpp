#include "includes/Angel.h"
#include <stdlib.h>
#include <cmath>

using namespace Angel;

typedef vec2 point2;


const int n_divisions = 8;
const int n_points = pow(3, n_divisions+1);

void triangle(point2 a, point2 b, point2 c, point2* points_arr)
{
	static int ii = 0;
	points_arr[ii++] = a;
	points_arr[ii++] = b;
	points_arr[ii++] = c;
}

void divide_triangle(point2 a, point2 b, point2 c, int k, point2* points_arr)
{
	if(k>0) {
		point2 ab = (a+b)/2.0;
		point2 ac = (a+c)/2.0;
		point2 bc = (b+c)/2.0;

		divide_triangle(a, ab, ac, k-1, points_arr);
		divide_triangle(c, ac, bc, k-1, points_arr);
		divide_triangle(b, bc, ab, k-1, points_arr);
	} else 
	{
		triangle(a, b, c, points_arr);
	}
}

void init() {
	point2 points[n_points];
	point2 vertices[3] = { point2(-1.0, -1.0), point2(0.0, 1.0), point2(1.0,-1.0) };
	divide_triangle(vertices[0], vertices[1], vertices[2], n_divisions, points);

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
	glDrawArrays(GL_TRIANGLES, 0, n_points);
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

