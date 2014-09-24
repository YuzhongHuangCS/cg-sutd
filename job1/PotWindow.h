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
		virtual void onClick(int button, int state, int x, int y);
		virtual void onMotion(int x, int y);

		void spin(int id = 0);
		void transColor(int id = 0);

	protected:
		void readDir();
		void readFile(std::string fileName);
		void compilePot();
		void compileFile();

	private:
		int target = 0;
		std::vector<std::string> objFileList;
		std::vector<int> displayListMap = {-1};
		
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
		float angleX = 0;
		float angleY = 0;

		int transRange[2] = {0, 0};

		int buttonID;
		int buttonState;
		int buttonX;
		int buttonY;

		float angleXOffset = angleX;
		float angleYOffset = angleY;
		float xOffset = viewPoint[0][0];
		float yOffset = viewPoint[0][1];
		float fOffset = fovy;

		const static int spinSpeed = 3;
		const static int spinInterval = 25;
		const static int transCount = 10;
		const static int transInterval = 25;
		const static int dollyRate = 30;
		const static int trackRate = 150;
		const static int tumbleRate = 10;
};

#endif // _POT_WINDOW_H_
