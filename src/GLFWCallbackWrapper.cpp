//
// Created by pmerku on 30/05/2021.
//

#include "GLFWCallbackWrapper.hpp"
#include "App.hpp"

using namespace opengl;

void GLFWCallbackWrapper::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
	_app->framebufferSizeCallback(window, width, height);
}

void GLFWCallbackWrapper::mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
	_app->mouseScrollCallback(window, xOffset, yOffset);
}

void GLFWCallbackWrapper::setApplication(App *app) {
	_app = app;
}

App *GLFWCallbackWrapper::_app = nullptr;
