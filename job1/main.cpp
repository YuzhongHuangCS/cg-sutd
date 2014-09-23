#include "glWindow.h"
#include "PotWindow.h"

PotWindow* windowPointer;

int main(int argc, char *argv[]) {
	PotWindow window(argc, argv);

	windowPointer = &window;

	window.create(500, 500, "SolidTeapot");

	return 0;
}
