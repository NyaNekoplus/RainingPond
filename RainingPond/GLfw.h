#ifndef GLFW_H_
#define GLFW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace OpenGL {
	class GLfw {
	public:
		static GLfw* GetInstance();
		void Init(size_t width = 800, size_t height = 600, const std::string& name = "RainingPool");
		GLFWwindow* GetWindow();
	private:
		GLfw();
		void LoadGL();
		void GenWindow(size_t width, size_t height, const std::string& name);
		static GLfw* instance;
		GLFWwindow* window;
	};
}
#endif