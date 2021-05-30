//
// Created by pmerku on 28/05/2021.
//

#include <exception>
#include <cstdlib>

#include "App.hpp"
#include "ErrorUtils.hpp"

int main(int argc, char *argv[]) {
	try {
		opengl::App	app;
		app.run();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
