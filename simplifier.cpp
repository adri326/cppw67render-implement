#include <iostream>
#include <string>
#include <fstream>
#include <openw67render/OpenContext.h>
#include <openw67render/window/OpenWindow.h>
#include <openw67render/graphics/OpenGraphics.h>
#include <glbinding/Binding.h>

using namespace std;
using namespace w67r;

OpenGraphics graphics;
OpenWindow swindow = OpenWindow("", 0, 0, OpenMonitor::PRIMARY_MONITOR);
int swidth = 0, sheight = 0;

void log(string s) {
	cout << s << endl;
}

void error_callback(int error, const char *description)
{
    fprintf(stderr, string("Error: %s\n" + to_string(error)).c_str(), description);
}

class SimpleRunnable:public Runnable {
	public:
		GLFWerrorfun glfwErrorCallback() {
			return error_callback;
		}
		void initError(string error) {

		}
		void init() {
			log("Initialised Runnable");
		}
		void glInit() {
			log("Initialised GL");
		}
};

OpenWindow createWindow(unsigned int width, unsigned int height) {
	SimpleRunnable runnable = SimpleRunnable();
	log("Initialising Context");
	Context context = Context(&runnable);
	context.init();
	OpenWindow window = OpenWindow("test", width, height, OpenMonitor::PRIMARY_MONITOR);
	swindow = window;
	if (!window.getWindowPointer()) cerr << "Error while initialising the window" << endl;
	log("Making current OpenGL context to window");
    glfwMakeContextCurrent(window.getWindowPointer());
    context.glInit();
    OpenGraphics graphics = OpenGraphics();
    swidth = width;
    sheight = height;
	return window;
}

void draw();

void runWindow(OpenWindow window) {

	while (!glfwWindowShouldClose(window.getWindowPointer())) {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	draw();
    	glfwSwapBuffers(window.getWindowPointer());
    	glfwPollEvents();
    }
}
float calcColor(float n, int t); // RED: t = 0, GREEN: t = 1, BLUE: t = 2, ALPHA: t = 3

void background(float r, float g, float b) {
	glClearColor(calcColor(r, 0), calcColor(g, 1), calcColor(b, 2), calcColor(1, 3));
}

void color(float r, float g, float b) {
	glColor3f(calcColor(r, 0), calcColor(g, 1), calcColor(b, 2));
}

float calcPosX(float n);
float calcPosY(float n);

void rect(float x1, float y1, float x2, float y2) {
	glBegin(GL_QUADS);
		glVertex3f(calcPosX(x1), calcPosY(y1), 0.0f);
		glVertex3f(calcPosX(x2), calcPosY(y1), 0.0f);
		glVertex3f(calcPosX(x2), calcPosY(y2), 0.0f);
		glVertex3f(calcPosX(x1), calcPosY(y2), 0.0f);
	glEnd();
}

void tri(float x1, float y1, float x2, float y2, float x3, float y3) {
	glBegin(GL_TRIANGLES);
		glVertex3f(calcPosX(x1), calcPosY(y1), 0.0f);
		glVertex3f(calcPosX(x2), calcPosY(y2), 0.0f);
		glVertex3f(calcPosX(x3), calcPosY(y3), 0.0f);
	glEnd();
}

void translate(float x, float y) {
	glTranslatef(x, y, 0);
}

void render2D() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // preserve perspective view
	glLoadIdentity(); // clear the perspective matrix
	glOrtho( // turn on 2D mode
		//// viewportX,viewportX+viewportW, // left, right
		//// viewportY,viewportY+viewportH, // bottom, top !!!
		0, swindow.getWidth(), // left, right
		swindow.getHeight(), 0, // bottom, top
	-500, 500); // Zfar, Znear
	setViewport(0, 0, swindow.getWidth(), swindow.getHeight());

	graphics.setTransparency(true);
	// clear the modelview matrix
	glMatrixMode(GL_MODELVIEW);
}

void pushMatrix() {
	glPushMatrix();
}
void popMatrix() {
	glPopMatrix();
}

char* loadShader(string name) {
	ifstream fin(name);
	// get pointer to associated buffer object
	filebuf* pbuf = fin.rdbuf();
	// get file size using buffer's members
	size_t size = pbuf->pubseekoff(0, fin.end, fin.in);
	pbuf->pubseekpos(0, fin.in);
	// allocate memory to contain file data
	char* buffer= new char[size];
	// get file data
	pbuf->sgetn(buffer, size);
	fin.close();
	buffer[size-1] = '\0';
	return buffer;
}

GLuint createShader(string name, GLenum type) {
	char* source = loadShader(name);
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	return shader;
}

GLuint createProgram(GLuint a, GLuint b) {
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, a);
	glAttachShader(shaderProgram, b);
	return shaderProgram;
}

float calcPosX(float n) {
	float r = n;
	r = n / swidth * 2 - 1;
	return r;
}
float calcPosY(float n) {
	float r = n;
	r = n / sheight * 2 - 1;
	return r;
}

float calcColor(float n, int t) {
	float r = n;
	if (t==3) { // ALPHA
		r = n;
	} else {
		r = n / 256;
	}
	return r;
}