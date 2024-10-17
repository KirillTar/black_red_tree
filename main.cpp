#include <iostream>
#include "App.h"

int main(int argc, char* argv[]) {

	A a;
	Application app;
	app.init();
	app.run();
	
	std::cout << "Work done" << std::endl;
	return EXIT_SUCCESS;
}