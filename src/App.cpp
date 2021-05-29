//
// Created by pmerku on 28/05/2021.
//

#include "App.hpp"
#include "ErrorUtils.hpp"

using namespace opengl;

App::App() = default;

App::~App() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void App::initialize() {
	if (!glfwInit()) {
		ERROR_THROW(InitException());
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		ERROR_THROW(WindowException());
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		ERROR_THROW(GladInitException());
	}

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void App::run() {

	while(!glfwWindowShouldClose(window)) {
		processInput(window);

		// TODO rendering here

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void App::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void App::processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
