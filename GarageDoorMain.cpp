#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <unistd.h>

#include "Machine.h"

void *RunInputScaner(void* args) {

	std::cout << "Input Scanner running..." << std::endl;

	std::cout << "Enter m for OVERCURRENT, i for IR_TRIP, r for PUSH_BUTTON then push enter." << std::endl;
	std::cout << "Enter s to shutdown the machine..." << std::endl;

	char inp;
	while(1)
	{
		std::cin >> inp;
		std::cout << "GOT " << inp << std::endl;

		switch(inp)
		{
		case 'm':
//			m1.HandleEvent(Controller::OVERCURRENT);
			break;
		case 'i':
//			m1.HandleEvent(Controller::IR_TRIP);
			break;
		case 'r':
//			m1.HandleEvent(Controller::PUSH_BUTTON);
			break;
		case 's':
			return EXIT_SUCCESS;
		default:
			std::cout << "Invalid input " << inp << std::endl;
		}
	}

	return EXIT_SUCCESS;
}

void *RunController(void* args) {
	Controller::Machine m1 = Controller::Machine();

	std::cout << "Controller running..." << std::endl;
	while(1){
		sleep(1);
	}

	return EXIT_SUCCESS;
}


int main(int argc, char *argv[]) {

	std::cout << "Main running... " << std::endl;

	pthread_t inputScannerThread;
	pthread_t controllerThread;

	pthread_create( &inputScannerThread, NULL, &RunInputScaner, (void*)&controllerThread );
	pthread_create( &controllerThread, NULL, &RunController, (void*)&inputScannerThread );

	pthread_join(inputScannerThread, NULL);
	pthread_kill(controllerThread, 0);

	return EXIT_SUCCESS;
}
