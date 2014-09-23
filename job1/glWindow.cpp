#include "glWindow.h"

glWindow::glWindow(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
}

glWindow::~glWindow() {
}

void glWindow::create(int width, int height, std::string title) {
	glutInitWindowSize(width, height);
	glutCreateWindow(title.c_str());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE_ARB);
	glEnable(GL_LIGHTING);
}

void glWindow::addLight(int index) {
	glEnable(GL_LIGHT0 + index);
}

void glWindow::setViewPoint(float viewPoint[3][3]) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		viewPoint[0][0], viewPoint[0][1], viewPoint[0][2],
		viewPoint[1][0], viewPoint[1][1], viewPoint[1][2],
		viewPoint[2][0], viewPoint[2][1], viewPoint[2][2]
	);
}

void glWindow::setMaterialDiffuse(float diffColor[3]) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor);
}

void glWindow::setMaterialSpecular(float specColor[3]) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
}

void glWindow::setMaterialShininess(float shininess[]) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void glWindow::setLightDiffuse(int index, float lightDiffColor[4]) {
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, lightDiffColor);
}

void glWindow::setLightPosition(int index, float lightPosition[4]) {
	glLightfv(GL_LIGHT0 + index, GL_POSITION, lightPosition);
}

void glWindow::drawText(std::string text) {
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, width, 0, height);

	glRasterPos2i(10, 10);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*) text.c_str());

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

void glWindow::onDraw() {
	// just clean screen and do nothing.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void glWindow::onReshape(int width, int height) {
	this->width = width;
	this->height = height;
	
	if (width > height) {
		glViewport(0, (height - width) / 2, width, width);
	} else {
		glViewport((width - height) / 2, 0, height, height);
	}

	// Set up a perspective view, with square aspect ratio
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 50 degree fov, uniform aspect ratio, near = 1, far = 100
	gluPerspective(fovy, 1.0, 1.0, 100.0);
	glutSwapBuffers();
}
