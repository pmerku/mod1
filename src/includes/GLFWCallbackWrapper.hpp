//
// Created by pmerku on 30/05/2021.
//

#ifndef GLFWCALLBACKWRAPPER_HPP
#define GLFWCALLBACKWRAPPER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace opengl {

	class App;

	class GLFWCallbackWrapper {
	public:
		GLFWCallbackWrapper() = delete;
		GLFWCallbackWrapper(const GLFWCallbackWrapper &obj) = delete;
		GLFWCallbackWrapper &operator=(const GLFWCallbackWrapper &obj) = delete;
		~GLFWCallbackWrapper() = delete;

		static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
		static void mousePositionCallback(GLFWwindow *window, double xPos, double yPos);
		static void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
		static void setApplication(App *app);

	private:
		static App *_app;
	};

} // namespace opengl

#endif //GLFWCALLBACKWRAPPER_HPP
