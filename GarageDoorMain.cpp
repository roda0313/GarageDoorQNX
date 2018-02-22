#include <cstdlib>
#include <iostream>

#include "Machine.h"

int main(int argc, char *argv[]) {

	Controller::Machine m1 = Controller::Machine();

	int num;
	while(1)
	{
		std::cout << "Please press enter..." << std::endl;
		std::cin >> num;
		m1.HandleEvent(Controller::PUSH_BUTTON);
	}

	return EXIT_SUCCESS;
}
