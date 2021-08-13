//
// Created by pmerku on 28/05/2021.
//

#include "App.hpp"
#include "ErrorUtils.hpp"
#include "Shader.hpp"

using namespace opengl;

App::App() {
	camera = new Camera();
	initialize();
}

App::~App() {
	delete _shader;
	delete camera;
	delete _terrain;
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		ERROR_THROW(GladInitException());
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glDepthFunc(GL_LESS);
}

void App::run() {
	// load vertex and fragment
	_shader = new Shader(PATH_TO_VERTEX, PATH_TO_FRAGMENT);
	_shader->use();

	// light parameters
	_shader->setVec3("light.ambient", 0.2, 0.2, 0.2);
	_shader->setVec3("light.diffuse", 0.3, 0.3, 0.3);
	_shader->setVec3("light.specular", 1.0, 1.0, 1.0);
	_shader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);

	_terrain = new Terrain();

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

void App::render() {
	// activate shader
	_shader->use();

	// create transformations
	_model.projection =
			glm::perspective(
			glm::radians(camera->zoom),
			(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
			0.1f,
			(float)_terrain->width * _terrain->renderDistance);

	_model.view = camera->getViewMatrix();
	_shader->setMat4("projection", _model.projection);
	_shader->setMat4("view", _model.view);
	_shader->setVec3("viewPos", camera->position);

	drawTerrain();
}

void App::drawTerrain() {
	// background color
	glClearColor(0.8f, 0.8f, 0.8f, 0.8f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_model.model = glm::mat4(1.0f);
	_model.model = glm::rotate(_model.model, glm::radians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	_model.model = glm::translate(_model.model, glm::vec3(-_terrain->width / 2.0 , 0.0, -_terrain->height / 2.0));
	_shader->setMat4("model", _model.model);

	glBindVertexArray(_terrain->map[0]);
	glDrawElements(GL_TRIANGLES, _terrain->nIndices, GL_UNSIGNED_INT, nullptr);
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

void App::mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
	camera->processMouseScroll(yOffset);
}

void App::mousePositionCallback(GLFWwindow *window, double xPos, double yPos) {
	if (_firstFrame) {
		_lastX = xPos;
		_firstFrame = false;
	}
	_xOffset = xPos - _lastX;
	_lastX = xPos;
	_xOffset *= camera->mouseSensitivity;
}

void App::processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// enable wireframe
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// disable wireframe
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// model rotation
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		_angle -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		_angle += 1.0f;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		_angle += _xOffset;
	}
}
