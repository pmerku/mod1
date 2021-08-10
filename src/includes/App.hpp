//
// Created by pmerku on 28/05/2021.
//

#ifndef APP_HPP
#define APP_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <exception>
#include <string>

#include "Shader.hpp"
#include "Camera.hpp"
#include "GLFWCallbackWrapper.hpp"
#include "Model.hpp"
#include "Terrain.hpp"

#define WINDOW_NAME "mod1"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define PATH_TO_VERTEX "./resources/vertex.glsl"
#define PATH_TO_FRAGMENT "./resources/fragment.glsl"

namespace opengl {

	class App {
	public:
		Camera	*camera{};

		App();
		~App();

		void initialize();
		void run();

		class AppException : public std::exception {
		public:
			virtual std::string what() noexcept {
				return "There was a problem with the app";
			}
		};

		class InitException : public AppException {
		public:
			std::string what() noexcept override {
				return "Couldn't initialize glfw";
			}
		};

		class WindowException : public AppException {
		public:
			std::string what() noexcept override {
				return "Couldn't create window";
			}
		};

		class GladInitException : public AppException {
		public:
			std::string what() noexcept override {
				return "Couldn't initialize glad";
			}
		};

	private:
		GLFWwindow	*window{};

		Shader		*_shader{};
		Model		_model;
		Terrain		*_terrain;

		float	_deltaTime = 0.0f;	// time between current frame and last frame
		float	_lastTime = 0.0f;
		int 	_nbFrames{};

		// terrain rotation
		float _angle = 0.0f;

		void render();
		void drawTerrain();
		void showFps();

		void setCallbackFunctions();
		void framebufferSizeCallback(GLFWwindow *window, int width, int height);
		void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
		void processInput(GLFWwindow *window);

		friend class GLFWCallbackWrapper;
	};

} // namespace opengl

#endif //APP_HPP
