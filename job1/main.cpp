#include "glWindow.h"
#include "PotWindow.h"

PotWindow* windowPointer;
void onDrawWrapper();
void onReshapreWrapper(int width, int height);
void onKeyPressWrapper(unsigned char key, int x, int y);
void onSpecialKeyPressWrapper(int key, int x, int y);

int main(int argc, char *argv[]) {
	PotWindow window(argc, argv);

	window.create(500, 500, "SolidTeapot");

	windowPointer = &window;

	glutDisplayFunc(onDrawWrapper);
	glutReshapeFunc(onReshapreWrapper);
	glutKeyboardFunc(onKeyPressWrapper);
    glutSpecialFunc(onSpecialKeyPressWrapper);
	glutMainLoop();

	return 0;
}

void onDrawWrapper(){
	windowPointer->onDraw();
}

void onReshapreWrapper(int width, int height){
	windowPointer->onReshape(width, height);
}

void onKeyPressWrapper(unsigned char key, int x, int y){
	windowPointer->onKeyPress(key, x, y);
}

void onSpecialKeyPressWrapper(int key, int x, int y){
	windowPointer->onSpecialKeyPress(key, x, y);
}