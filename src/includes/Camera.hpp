//
// Created by pmerku on 29/05/2021.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace opengl {

#define YAW (-90.0f)
#define PITCH (-20.0f)
#define ZOOM (45.0f)

	class Camera {
	public:
		float 		zoom{};
		glm::vec3	position{};

		Camera();
		~Camera();

		glm::mat4 getViewMatrix();
		void processMouseScroll(float yOffset);

	private:
		glm::vec3 _front{};
		glm::vec3 _up{};
		glm::vec3 _right{};
		glm::vec3 _worldUp{};

		float _yaw;
		float _pitch;

		void updateCameraVectors();
	};

} // namespace opengl

#endif //CAMERA_HPP
