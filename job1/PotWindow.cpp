#include <fstream>
#include <boost/algorithm/string.hpp>
#include "PotWindow.h"

extern PotWindow* windowPointer;

void onDrawWrapper();
void onReshapreWrapper(int width, int height);
void onKeyPressWrapper(unsigned char key, int x, int y);
void onSpecialKeyPressWrapper(int key, int x, int y);
void spinWrapper(int id);
void transColorWrapper(int id);
void onClickWrapper(int button, int state, int x, int y);
void onMotionWrapper(int x, int y);

PotWindow::PotWindow(int argc, char** argv):
	glWindow(argc, argv)
{
}

PotWindow::~PotWindow() {
}

void PotWindow::create(int width, int height, std::string title) {
	glWindow::create(width, height, title);

	addLight(0);
	setLightDiffuse(0, lightDiff);
	setLightPosition(0, lightPosition);

	setMaterialDiffuse(diffColors[diffColorIndex]);
	setMaterialSpecular(specColor);
	setMaterialShininess(shininess);

	glutDisplayFunc(::onDrawWrapper);
	glutReshapeFunc(::onReshapreWrapper);
	glutKeyboardFunc(::onKeyPressWrapper);
    glutSpecialFunc(::onSpecialKeyPressWrapper);
    glutMouseFunc(::onClickWrapper);
    glutMotionFunc(::onMotionWrapper);

    glutMainLoop();
}

void PotWindow::onDraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setViewPoint(viewPoint);

	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);

	if(displayListMap[target] == -1){
		if(target == 0){
			compilePot();
		} else{
			compileFile();
		}
	} else{
		glCallList(displayListMap[target]);
	}

	if(!statusText.empty()) {
		drawText(statusText);
		statusText.clear();
	}

	glutSwapBuffers();
}

void PotWindow::compilePot(){
	int listID = glGenLists(1);
	
	glNewList(listID, GL_COMPILE_AND_EXECUTE);
		glutSolidTeapot(1.0);
	glEndList();

	displayListMap[0] = listID;
}

void PotWindow::readFile(std::string fileName) {
	std::ifstream inFile(fileName);
	std::string line;
	std::vector<std::string> words;
	std::vector<float> nums;
	std::vector<int> intNums;
	
	vertex.clear();
	normal.clear();
	face.clear();

	if(!inFile.is_open()) {
		statusText = "Unable to open file " + fileName;
		std::cerr << statusText << std::endl;
		return;
	}

	while (std::getline(inFile, line)) {
		if(line.find("v ") != std::string::npos){
			boost::split(words, line, boost::is_any_of(" "));
			nums.clear();
			// index start from 1 to exclude first item
			for(unsigned int i = 1; i < words.size(); i++) {
				nums.push_back(std::stof(words[i]));
			}
			vertex.push_back(nums);
			continue;
		}

		if(line.find("vn ") != std::string::npos){
			boost::split(words, line, boost::is_any_of(" "));
			nums.clear();
			for(unsigned int i = 1; i < words.size(); i++) {
				nums.push_back(std::stof(words[i]));
			}
			normal.push_back(nums);
			continue;
		}

		if(line.find("f ") != std::string::npos){
			boost::split(words, line, boost::is_any_of(" /"));
			// adjust the offset of index here
			intNums.clear();
			for(unsigned int i = 1; i < words.size(); i++) {
				intNums.push_back(std::stoi(words[i]) - 1);
			}
			face.push_back(intNums);
			continue;
		}
	}
}

void PotWindow::compileFile() {
	int listID = glGenLists(1);
	glNewList(listID, GL_COMPILE_AND_EXECUTE);
	
	for(auto it = face.begin(); it != face.end(); it++){
		std::vector<int> item = *it;
		glBegin(GL_TRIANGLES);
			glNormal3f(normal[item[2]][0], normal[item[2]][1], normal[item[2]][2]);
			glVertex3f(vertex[item[0]][0], vertex[item[0]][1], vertex[item[0]][2]);
			glNormal3f(normal[item[5]][0], normal[item[5]][1], normal[item[5]][2]);
			glVertex3f(vertex[item[3]][0], vertex[item[3]][1], vertex[item[3]][2]);
			glNormal3f(normal[item[8]][0], normal[item[8]][1], normal[item[8]][2]);
			glVertex3f(vertex[item[6]][0], vertex[item[6]][1], vertex[item[6]][2]);
		glEnd();
	}
	
	glEndList();
	displayListMap[target] = listID;
}

void PotWindow::onKeyPress(unsigned char key, int x, int y) {
	switch(key) {
		case 27:
			// Escape key
			exit(0);
			break;
		case 'c':
			transRange[0] = diffColorIndex;
			transRange[1] = diffColorIndex = (diffColorIndex + 1) % 4;
			transColor(0);
			break;
		case 'l':
			target = (target + 1) % 4;
			if((target != 0) && (displayListMap[target] == -1)){
				readFile(objFileList[target-1]);
			}
			break;
		case 's':
			spinning = !spinning;
			if(spinning){
				spin();
			}
			break;
	}

	glutPostRedisplay();
}

void PotWindow::onSpecialKeyPress(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_UP:
			lightPosition[1] += 0.5;
			break;
		case GLUT_KEY_DOWN:
			lightPosition[1] -= 0.5;
			break;
		case GLUT_KEY_LEFT:
			lightPosition[0] -= 0.5;
			break;
		case GLUT_KEY_RIGHT:
			lightPosition[0] += 0.5;
			break;
	}

	setLightPosition(0, lightPosition);
	glutPostRedisplay();
}

void PotWindow::spin(int id) {
	angleY += 5;
	glutPostRedisplay();
	if(spinning){
		glutTimerFunc(40, spinWrapper, 1);
	}
}

void PotWindow::transColor(int id) {
	float currentColor[4] = {
		diffColors[transRange[0]][0] + (diffColors[transRange[1]][0] - diffColors[transRange[0]][0]) / 10 * id,
		diffColors[transRange[0]][1] + (diffColors[transRange[1]][1] - diffColors[transRange[0]][1]) / 10 * id,
		diffColors[transRange[0]][2] + (diffColors[transRange[1]][2] - diffColors[transRange[0]][2]) / 10 * id,
		diffColors[transRange[0]][3] + (diffColors[transRange[1]][3] - diffColors[transRange[0]][3]) / 10 * id
	};

	setMaterialDiffuse(currentColor);
	glutPostRedisplay();
	
	if(id < 10){
		glutTimerFunc(25, transColorWrapper, ++id);
	}
}

void PotWindow::onClick(int button, int state, int x, int y){
	buttonID = button;
	buttonState = state;
	if(state == 1){
		if(buttonID == 0){
			angleXOffset = angleXOffset + float(y-buttonY) / 10;
			angleYOffset = angleYOffset + float(x-buttonX) / 10;
		}
		if(buttonID == 1){
			xOffset += float((buttonX-x)) / 100;
			yOffset += float((y-buttonY)) / 100;
		}
		if(buttonID == 2){
			fOffset += float((buttonX-x) + (buttonY-y)) / 30;
		}
	}
	buttonX = x;
	buttonY = y;
}

void PotWindow::onMotion(int x, int y){
	if(buttonID == 0){
		// note that x and y used in screen and coordinate is different
		angleX = angleXOffset + float(y-buttonY) / 10;
		angleY = angleYOffset + float(x-buttonX) / 10;
		glutPostRedisplay();
	}
	if(buttonID == 1){
		viewPoint[0][0] = xOffset + float((buttonX-x)) / 100;
		viewPoint[1][0] = xOffset + float((buttonX-x)) / 100;
		viewPoint[0][1] = yOffset + float((y-buttonY)) / 100;
		viewPoint[1][1] = yOffset + float((y-buttonY)) / 100;
		glutPostRedisplay();
	}
	if(buttonID == 2){
		fovy = fOffset + float((buttonX-x) + (buttonY-y)) / 30;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fovy, 1.0, 1.0, 100.0);
		glutPostRedisplay();
	}
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

void spinWrapper(int id) {
	windowPointer->spin(id);
}

void transColorWrapper(int id) {
	windowPointer->transColor(id);
}

void onClickWrapper(int button, int state, int x, int y) {
	windowPointer->onClick(button, state, x, y);
}

void onMotionWrapper(int x, int y) {
	windowPointer->onMotion(x, y);
}