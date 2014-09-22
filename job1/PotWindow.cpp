#include <fstream>
#include <boost/algorithm/string.hpp>
#include "PotWindow.h"
#include <cmath>

#define PI 3.14159265358979323846

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
}

void PotWindow::onDraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(target == 0){
		drawPot();
	} else{
		drawFile();
	}

	if(!statusText.empty()) {
		drawText(statusText);
		statusText.clear();
	}

	glutSwapBuffers();
}

void PotWindow::drawPot(){
	setViewPoint(viewPoint);
	glutSolidTeapot(1.0);
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

void PotWindow::drawFile() {
	setViewPoint(viewPoint);

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
}

void PotWindow::onKeyPress(unsigned char key, int x, int y) {
	switch(key) {
		case 27:
			// Escape key
			exit(0);
			break;
		case 'c':
			diffColorIndex = (diffColorIndex + 1) % 4;
			setMaterialDiffuse(diffColors[diffColorIndex]);
			break;
		case 'l':
			target = (target + 1) % 4;
			if(target != 0){
				readFile(objFileList[target-1]);
			}
			break;
		case 's':
			//spinning = !spinning;
			//if(spinning){
			spin();
			//}
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

void PotWindow::spin(int id){
	angle += PI/60;

	viewPoint[0][0] = distance * sin(angle);
	viewPoint[0][2] = distance * cos(angle);

	glutPostRedisplay();
	//glutTimerFunc(100, this->spin, 1);
}