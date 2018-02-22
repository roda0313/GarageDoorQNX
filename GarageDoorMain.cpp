#include <cstdlib>
#include <iostream>

#include "Machine.h"

int main(int argc, char *argv[]) {

	Controller::Machine m1 = Controller::Machine();

	char inp;
	while(1)
	{
		std::cout << "Enter m, i or r then push enter..." << std::endl;
		std::cin >> inp;

		switch(inp)
		{
		case 'm':
			m1.HandleEvent(Controller::OVERCURRENT);
			break;
		case 'i':
			m1.HandleEvent(Controller::IR_TRIP);
			break;
		case 'r':
			m1.HandleEvent(Controller::PUSH_BUTTON);
			break;
		default:
			std::cout << "Invalid input" << inp << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
