//
// Created by pmerku on 28/05/2021.
//

#include "App.hpp"
#include "ErrorUtils.hpp"
#include "Shader.hpp"
#include "constants.hpp"

using namespace opengl;

App::App() {
	camera = new Camera();
	initialize();
}

App::~App() {
	delete _shader;
	delete camera;
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
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
		glfwTerminate();
		ERROR_THROW(WindowException());
	}
	glfwMakeContextCurrent(window);
	setCallbackFunctions();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		ERROR_THROW(GladInitException());
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void App::run() {
	// load vertex and fragment
	_shader = new Shader(PATH_TO_VERTEX, PATH_TO_FRAGMENT);
	setupVertexData();

	while(!glfwWindowShouldClose(window)) {
		// Measure speed
		showFps();

		// input handler
		processInput(window);

		// rendering
		render();

		// swap buffers and poll IO events (keyboard, mouse..)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void App::setupVertexData() {
	// setup vertex data, buffers and attributes
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void App::render() {
	// background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// activate shader
	_shader->use();

	// create transformations
	_model.projection = glm::perspective(glm::radians(camera->zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	_model.view = camera->getViewMatrix();
	_model.model = glm::mat4(1.0f);

	_model.mvp = _model.projection * _model.view * _model.model;
	_shader->setMat4("mvp", _model.mvp);

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}

void App::showFps() {
	auto currentTime = glfwGetTime();
	_deltaTime = currentTime - _lastTime;
	_nbFrames++;
	if (_deltaTime >= 1.0) {
		double fps = double(_nbFrames) / _deltaTime;

		std::stringstream ss;
		ss << WINDOW_NAME << " [" << fps << " FPS]";
		std::string title = ss.str();

		glfwSetWindowTitle(window, title.c_str());

		_nbFrames = 0;
		_lastTime = currentTime;
	}
}

void App::setCallbackFunctions() {
	GLFWCallbackWrapper::setApplication(this);
	glfwSetFramebufferSizeCallback(window, GLFWCallbackWrapper::framebufferSizeCallback);
	glfwSetCursorPosCallback(window, GLFWCallbackWrapper::mousePositionCallback);
	glfwSetScrollCallback(window, GLFWCallbackWrapper::mouseScrollCallback);
}

void App::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void App::mousePositionCallback(GLFWwindow *window, double xPos, double yPos) {
	if (_firstFrame) {
		_lastX = xPos;
		_firstFrame = false;
	}
	float xOffset = xPos - _lastX;
	_lastX = xPos;

	camera->processMouseMovement(xOffset);
}

void App::mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
	camera->processMouseScroll(yOffset);
}

void App::processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->processKeyboard(FORWARD, _deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->processKeyboard(BACKWARD, _deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->processKeyboard(LEFT, _deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->processKeyboard(RIGHT, _deltaTime);
}
