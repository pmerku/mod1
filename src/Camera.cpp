//
// Created by pmerku on 29/05/2021.
//

#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace opengl;

Camera::Camera()
	: position(glm::vec3(0.0f, 60.0f, 140.0f)),
	  _worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	  _front(glm::vec3(0.0f, 0.0f, -1.0f)),
	  _yaw(YAW),
	  _pitch(PITCH),
	  zoom(ZOOM)
{
	updateCameraVectors();
}

Camera::~Camera() = default;

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + _front, _up);
}

void Camera::processMouseScroll(float yOffset) {
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yOffset;
	if (zoom < 20.0f)
		zoom = 20.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);
	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}


