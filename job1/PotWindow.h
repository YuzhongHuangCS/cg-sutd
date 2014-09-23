#ifndef _POT_WINDOW_H_
#define _POT_WINDOW_H_

#include "glWindow.h"
#include <vector>

class PotWindow : public glWindow {
	public:
		PotWindow(int argc, char** argv);
		virtual ~PotWindow();

		virtual void create(int width = 300, int height = 300, std::string title = "glWindow");
		virtual void onDraw();
		virtual void onKeyPress(unsigned char key, int x, int y);
		virtual void onSpecialKeyPress(int key, int x, int y);

		void spin(int id = 0);
		void transColor(int id = 0);

	protected:
		void readFile(std::string fileName);
		void compilePot();
		void compileFile();

	private:
		int target = 0;
		std::string objFileList[3] = {"garg.obj", "sphere.obj", "torus.obj"};

		float viewPoint[3][3] = {
			{0.0, 0.0, 5.0},
			{0.0, 0.0, 0.0},
			{0.0, 1.0, 0.0}
		};
		float diffColors[4][4] = {
			{0.5, 0.5, 0.9, 1.0},
			{0.9, 0.5, 0.5, 1.0},
			{0.5, 0.9, 0.3, 1.0},
			{0.3, 0.8, 0.9, 1.0}
		};
		int diffColorIndex = 0;
		float specColor[4] = {1.0, 1.0, 1.0, 1.0};
		float shininess[1] = {100.0};
		float lightDiff[4] = {1.0, 1.0, 1.0, 1.0};
		float lightPosition[4] = {1.0, 1.0, 5.0, 1.0};

		std::vector< std::vector<float> > vertex;
		std::vector< std::vector<float> > normal;
		std::vector< std::vector<int> > face;

		std::string statusText;
		bool spinning = false;
		float angle = 5;

		int displayListMap[4] = {-1, -1, -1, -1};
		int transRange[2] = {0, 0};
};

#endif // _POT_WINDOW_H_