#ifndef _GL_WINDOW_H_
#define _GL_WINDOW_H_

#include <GL/freeglut.h>
#include <string>
#include <iostream>

class glWindow {
	public:
		glWindow(int argc, char** argv);
		virtual ~glWindow();
		
		void addLight(int index);

		void setViewPoint(float viewPoint[3][3]);
		void setMaterialDiffuse(float diffColor[4]);
		void setMaterialSpecular(float specColor[]);
		void setMaterialShininess(float shininess[]);
		void setLightDiffuse(int index, float lightDiffColor[4]);
		void setLightPosition(int index, float lightPosition[4]);

		void drawText(std::string text);

		virtual void create(int width = 300, int height = 300, std::string title = "glWindow");
		virtual void onDraw();
		virtual void onReshape(int width, int height);

	protected:

	private:

};

#endif // _GL_WINDOW_H_