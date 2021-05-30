//
// Created by pmerku on 30/05/2021.
//

#ifndef MODEL_HPP
#define MODEL_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace opengl {

	class Model {
	public:
		Model();
		~Model();

		glm::mat4 projection{};
		glm::mat4 view{};
		glm::mat4 model{};
		glm::mat4 mvp{};
	};

}

#endif //MODEL_HPP
