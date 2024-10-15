#include "includes/Angel.h"
#include <stdlib.h>
#include <cmath>

using namespace Angel;

typedef vec3 point3;
typedef vec3 color3;

const int n_divisions = 2;
const int n_points = pow(12, n_divisions+1);
point3 points[n_points];
color3 colors[n_points];
color3 base_colors[4] = {color3(0.7,0.3,0.5), color3(0.4,0.9,0.2), color3(0.6,0.2,0.4), color3(0.4,0.6,0.2)};
int color_idx = 0;

static int ii = 0;
void triangle(point3 a, point3 b, point3 c, point3* points_arr)
{
	colors[ii] = base_colors[color_idx];
	points_arr[ii++] = a;
	colors[ii] = base_colors[color_idx];
	points_arr[ii++] = b;
	colors[ii] = base_colors[color_idx];
	points_arr[ii++] = c;
}

void tetra(point3 a, point3 b, point3 c, point3 d, point3* points_arr) {
	color_idx = 0;
	triangle(a, b, c, points_arr);
	color_idx = 1;
	triangle(a, c, d, points_arr);
	color_idx = 2;
	triangle(a, d, b, points_arr);
	color_idx = 3;
	triangle(b, d, c, points_arr);
}

void divide_tetra(point3 a, point3 b, point3 c, point3 d, int k, point3* points_arr)
{
	if(k>0) {
		point3 mid[6];

		mid[0] = (a+b)/2.0;
		mid[1] = (a+c)/2.0;
		mid[2] = (a+d)/2.0;
		mid[3] = (b+c)/2.0;
		mid[4] = (c+d)/2.0;
		mid[5] = (d+b)/2.0;

		divide_tetra(a, mid[0], mid[1], mid[2], k-1, points_arr);
		divide_tetra(mid[0], b, mid[3], mid[5], k-1, points_arr);
		divide_tetra(mid[1], mid[3], c, mid[4], k-1, points_arr);
		divide_tetra(mid[2], mid[5], mid[4], d, k-1, points_arr);
	} else 
	{
		tetra(a, b, c, d, points_arr);
	}
}

void init() {
	point3 vertices[4] = { point3(-1.0, -1.0, -1.0), point3(1.0, -1.0, -1.0), point3(0.0, 1.0,-1.0), point3(0.0, 0.0, 1.0) };
	divide_tetra(vertices[0], vertices[1], vertices[2], vertices[3],  n_divisions, points);

	GLuint abuffer;
	glGenVertexArrays(1, &abuffer);
	glBindVertexArray(abuffer);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
}

void displayFunc() {
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, n_points);
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Sierpinski Gasket 3D");
	glewInit();
	glutDisplayFunc(displayFunc);
	init();

	GLuint program = InitShader("vsource_3d.glsl", "fsource_3d.glsl");
	GLuint loc =glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	GLuint loc2 = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc2);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
	
	glutMainLoop();
}

