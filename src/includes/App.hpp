//
// Created by pmerku on 28/05/2021.
//

#ifndef APP_HPP
#define APP_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <string>

#define WINDOW_NAME "mod1"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

namespace opengl {

	class App {
	private:
		GLFWwindow		*window{};

		static void	framebufferSizeCallback(GLFWwindow *window, int width, int height);
		static void	processInput(GLFWwindow *window);

	public:
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
	};

} // namespace opengl

#endif //APP_HPP
