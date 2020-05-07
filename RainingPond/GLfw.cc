#include "GLfw.h"
#include <iostream>
#include "Camera.h"

using namespace OpenGL;
using namespace std;

GLfw::GLfw():window(NULL){}
GLfw* GLfw::instance = new GLfw();
GLfw* GLfw::GetInstance() { return instance; }

void GLfw::Init(size_t width, size_t height, const std::string& name) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SAMPLES, 4);
	GenWindow(width,height,name);
	LoadGL();
	glViewport(0, 0, width, height);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scanCode, int state, int mods) {
		switch (key)
		{
		case GLFW_KEY_W:
			break;
		case GLFW_KEY_S:
			break;
		case GLFW_KEY_A:
			break;
		case GLFW_KEY_D:
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
		default:
			break;
		}
		
		Camera::GetInstance()->ProcessKeyboard(key);
	});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
		static double lastX, lastY;
		if (Camera::firstMouse) {
			lastX = xPos;
			lastY = yPos;
			Camera::firstMouse = false;
		}
		double xOffset = xPos - lastX;
		double yOffset = lastY - yPos;

		lastX = xPos;
		lastY = yPos;
		Camera::GetInstance()->ProcessMouseMovement(xOffset, yOffset);
	});
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
		Camera::GetInstance()->ProcessMouseScroll(yOffset);
	});
}
void GLfw::GenWindow(size_t width, size_t height, const std::string& name) {
	window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window,int width, int height) {
		glViewport(0, 0, width, height);
	});
}
void GLfw::LoadGL() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		exit(-1);
	}
}

GLFWwindow* GLfw::GetWindow() {return window;}