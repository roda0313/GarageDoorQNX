#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <unistd.h>
#include <string.h>

#include "Machine.h"

Controller::Events CharToEvent(char input) {
	switch(input) {
	case 'r':
		return Controller::PUSH_BUTTON;
	case 'm':
		return Controller::OVERCURRENT;
	case 'i':
		return Controller::IR_TRIP;
	case 's':
		return Controller::SHUTDOWN;
	default:
		return (Controller::Events)input;
	}
}

void *RunDoorOpeningTimerThread(void* args) {
	std::cout << "RunDoorOpeningTimerThread started..." << std::endl;

	int chid = (int)args;
	int coid;
	coid = ConnectAttach(0, getpid(), chid, 0, 0);
	char rmsg[200];
	if (coid == -1) {
		std::cout << "Couldn't Connect :(" << std::endl;
		return 0;
	}

	// actual timer
	for(int i = 0; i < 10; i++) {
		sleep(1);
	}

	// send an event saying its done
	// 9 = door closed event
	if(MsgSend(coid, "3", strlen("3") + 1, rmsg, sizeof(rmsg)) == -1) {
		std::cout << "Could not send message :[ " << "3" << std::endl;
	}

	return EXIT_SUCCESS;
}

void *RunDoorClosingTimerThread(void* args) {
	std::cout << "RunDoorClosingTimerThread started..." << std::endl;

	int chid = (int)args;
	int coid;
	coid = ConnectAttach(0, getpid(), chid, 0, 0);
	char rmsg[200];
	if (coid == -1) {
		std::cout << "Couldn't Connect :(" << std::endl;
		return 0;
	}

	for(int i = 0; i < 10; i++) {
		sleep(1);
	}

	// send an event saying its done
	// 4 = door closed event
	if(MsgSend(coid, "4", strlen("4") + 1, rmsg, sizeof(rmsg)) == -1) {
		std::cout << "Could not send message :[ " << "4" << std::endl;
	}

	return EXIT_SUCCESS;
}

void *RunInputScaner(void* args) {

	std::cout << "Input Scanner running..." << std::endl;

	int chid = (int)args;
	int coid;
	coid = ConnectAttach(0, getpid(), chid, 0, 0);
	char rmsg[200];
	if (coid == -1) {
		std::cout << "Couldn't Connect :(" << std::endl;
		return 0;
	}

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
		{
			if(MsgSend(coid, &inp, strlen(&inp) + 1, rmsg, sizeof(rmsg)) == -1) {
				std::cout << "Could not send message :[ " << inp << std::endl;
			}
			break;
		}
		case 'i':
		{
			if(MsgSend(coid, &inp, strlen(&inp) + 1, rmsg, sizeof(rmsg)) == -1) {
				std::cout << "Could not send message :[ " << inp << std::endl;
			}
			break;
		}
		case 'r':
		{
			if(MsgSend(coid, &inp, strlen(&inp) + 1, rmsg, sizeof(rmsg)) == -1) {
				std::cout << "Could not send message :[ " << inp << std::endl;
			}
			break;
		}
		case 's':
		{
			if(MsgSend(coid, &inp, strlen(&inp) + 1, rmsg, sizeof(rmsg)) == -1) {
				std::cout << "Could not send message :[ " << inp << std::endl;
			}
			return EXIT_SUCCESS;
		}
		default:
			std::cout << "Invalid input " << inp << std::endl;
		}
	}

	return EXIT_SUCCESS;
}

void *RunController(void* args) {
	int chid = (int)args;
	Controller::Machine m1 = Controller::Machine(chid);

	pthread_t doorOpeningTimerThread;
	pthread_t doorClosingTimerThread;
	bool openTimerRunning = false;
	bool closeTimerRunning = false;

	std::cout << "Controller running..." << std::endl;

	char message[512];
	int rcvid;

	while(1){
		// check for message
		rcvid = MsgReceive(chid, message, sizeof(message), NULL);
		std::cout << "GOT message in controller " << message << std::endl;

		// get the event
		Controller::Events e = CharToEvent(message[0]);

		// send a response
		strcpy(message, "Sending response");
		MsgReply(rcvid, 1, message, sizeof(message));

		switch(e) {
		case Controller::SHUTDOWN:
			std::cout << "Shutting down controller..." << std::endl;
			return EXIT_SUCCESS;
		case Controller::START_OPEN_TIMER:
			openTimerRunning = true;
			std::cout << "Start open timer" << std::endl;
//			pthread_create( &doorOpeningTimerThread, NULL, &RunDoorOpeningTimerThread, (void*)chid );
			break;
		case Controller::START_CLOSE_TIMER:
			closeTimerRunning = true;
			std::cout << "Start close timer" << std::endl;
//			pthread_create( &doorClosingTimerThread, NULL, &RunDoorClosingTimerThread, (void*)chid );
			break;
		case Controller::DOOR_OPENED:
			openTimerRunning = false;
			m1.HandleEvent(e);
			break;
		case Controller::DOOR_CLOSED:
			closeTimerRunning = false;
			m1.HandleEvent(e);
			break;
		default:
			// if either timer is running, kill it
//			if(openTimerRunning){
//				pthread_kill(doorOpeningTimerThread, 0);
//				openTimerRunning = false;
//			}
//
//			if(closeTimerRunning) {
//				pthread_kill(doorClosingTimerThread, 0);
//				closeTimerRunning = false;
//			}
			m1.HandleEvent(e);
			break;
		}
	}

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

	std::cout << "Main running... " << std::endl;

	pthread_t inputScannerThread;
	pthread_t controllerThread;

	int chid;
	chid = ChannelCreate(0);

	pthread_create( &inputScannerThread, NULL, &RunInputScaner, (void*)chid );
	pthread_create( &controllerThread, NULL, &RunController, (void*)chid );

	pthread_join(inputScannerThread, NULL);
	pthread_join(controllerThread, NULL);

	return EXIT_SUCCESS;
}
