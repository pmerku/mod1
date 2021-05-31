//
// Created by pmerku on 29/05/2021.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace opengl {

#define YAW (-90.0f)
#define PITCH (-18.0f)
#define SPEED (0.1f)
#define SENSITIVITY (0.01f)
#define ZOOM (45.0f)

	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera {
	public:
		float 		zoom{};
		glm::vec3	position{};

		Camera();
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~Camera();

		glm::mat4 getViewMatrix();
		void processKeyboard(CameraMovement direction, float deltaTime);
		void processMouseMovement(float xOffset);
		void processMouseScroll(float yOffset);

	private:
		glm::vec3 _front{};
		glm::vec3 _up{};
		glm::vec3 _right{};
		glm::vec3 _worldUp{};

		float _yaw;
		float _pitch;

		float _movementSpeed{};
		float _mouseSensitivity{};

		void updateCameraVectors();
	};

} // namespace opengl

#endif //CAMERA_HPP
