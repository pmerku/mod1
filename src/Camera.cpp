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
	  _movementSpeed(SPEED),
	  _mouseSensitivity(SENSITIVITY),
	  zoom(ZOOM)
{
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
	position = glm::vec3(posX, posY, posZ);
	_worldUp = glm::vec3(upX, upY, upZ);
	_yaw = yaw;
	_pitch = pitch;
	updateCameraVectors();
}

Camera::~Camera() = default;

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + _front, _up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
	float velocity = _movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += _front * velocity;
	if (direction == BACKWARD)
		position -= _front * velocity;
	if (direction == LEFT)
		position -= _right * velocity;
	if (direction == RIGHT)
		position += _right * velocity;
	position.y = 60.0f;
}

void Camera::processMouseMovement(float xOffset) {
	xOffset *= _mouseSensitivity;
	_yaw += xOffset;

	updateCameraVectors();
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


