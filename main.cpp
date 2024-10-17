#include <iostream>
#include "App.h"

template <typename T>
class A {
	
	A() {
	}

public:
	static A(T t) {
		x = t;
	}

	T x;
};

A a(1.0);

int main(int argc, char* argv[]) {

	Application app;
	app.init();
	app.run();
	
	std::cout << "Work done" << std::endl;
	return EXIT_SUCCESS;
}