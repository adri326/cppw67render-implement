#include <iostream>
#include <string>
#include <openw67render/window/OpenWindow.h>
#include <openw67render/graphics/OpenGraphics.h>
#include <glbinding/Binding.h>
#include "simplifier.cpp"

using namespace std;
using namespace w67r;
void draw() {
	background(0,0,0);
	color(16, 16, 16);
	rect(2, 2, 298, 298);
	color(255, 0, 0);
	tri(10, 10, 210, 10, 10, 210);
	color(255, 64, 0);
	tri(50, 50, 250, 50, 50, 250);
	color(255, 128, 0);
	tri(90, 90, 290, 90, 90, 290);
}

int main() {
	log("Starting program");
	OpenWindow window = createWindow(300, 300);
    runWindow(window);
    log("Ending program");
	return 0;
}