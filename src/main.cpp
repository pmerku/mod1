//
// Created by pmerku on 28/05/2021.
//

#include <exception>
#include <cstdlib>

#include "App.hpp"
#include "ErrorUtils.hpp"

class MainProcess {
public:
	opengl::App		app{};

	MainProcess() {
		app.initialize();
	}

	int run(int argc, char *argv[]) {
		try {
			// TODO parse input file
			app.run();
		} catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}

	~MainProcess() = default;
};

int main(int argc, char *argv[]) {
	MainProcess	process{};
	return process.run(argc, argv);
}
