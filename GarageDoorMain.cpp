#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <unistd.h>
#include <string.h>

#include "Machine.h"

struct Channels {
	int inputScannerChid;
	int controllerChid;
	int hardwareFacadeChid;
};

Controller::Events CharToEvent(char input) {
	switch(input) {
	case 'r':
	case '0':
		return Controller::PUSH_BUTTON;
	case 'i':
	case '1':
		return Controller::IR_TRIP;
	case 'm':
	case '2':
		return Controller::OVERCURRENT;
	case '3':
		return Controller::DOOR_OPENED;
	case '4':
		return Controller::DOOR_CLOSED;
	case '5':
		return Controller::IR_ON;
	case '6':
		return Controller::MOTOR_FORWARD;
	case '7':
		return Controller::MOTOR_REVERSE;
	case 's':
	case '8':
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
	char toSend = Controller::EventToChar(Controller::DOOR_OPENED);
	if(MsgSend(coid, &toSend, strlen(&toSend) + 1, rmsg, sizeof(rmsg)) == -1) {
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
	char toSend = Controller::EventToChar(Controller::DOOR_CLOSED);
	if(MsgSend(coid, &toSend, strlen(&toSend) + 1, rmsg, sizeof(rmsg)) == -1) {
		std::cout << "Could not send message :[ " << "4" << std::endl;
	}

	return EXIT_SUCCESS;
}

void *RunKeyboardScannerThread(void* args){
	std::cout << "Keyboard Scanner running..." << std::endl;

	Channels* chids = (Channels*)args;
	int chid = chids->inputScannerChid;
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

void *RunInputScaner(void* args) {

	std::cout << "Input Scanner running..." << std::endl;

	Channels* chids = (Channels*)args;
	int sendChid = chids->controllerChid;
	int receiveChid = chids->inputScannerChid;

	// connect to the channel we will send on
	int coid;
	coid = ConnectAttach(0, getpid(), sendChid, 0, 0);
	char rmsg[200];
	if (coid == -1) {
		std::cout << "Couldn't Connect :(" << std::endl;
		return 0;
	}

	char message[512];
	int rcvid;

	while(1){
		// check for message
		rcvid = MsgReceive(receiveChid, message, sizeof(message), NULL);
		std::cout << "GOT message in input scanner " << message << std::endl;

		// send a response to wherever the message originally came from
		std::string response =  "Sending response";
		MsgReply(rcvid, 1, &response, sizeof(response));

		// forward to controller
		if(MsgSend(coid, message, strlen(message) + 1, rmsg, sizeof(rmsg)) == -1) {
			std::cout << "Could not send message :[ " << message << std::endl;
		}
	}

	return EXIT_SUCCESS;

}

void *RunController(void* args) {
	Channels* chids = (Channels*)args;
	int receiveChid = chids->controllerChid;
	int sendChid = chids->hardwareFacadeChid;

	Controller::Machine m1 = Controller::Machine(sendChid);

	pthread_t doorOpeningTimerThread;
	pthread_t doorClosingTimerThread;
	bool openTimerRunning = false;
	bool closeTimerRunning = false;

	std::cout << "Controller running..." << std::endl;

	char message[512];
	int rcvid;

	while(1){
		// check for message
		std::cout << "Now waiting in controller for a message" << std::endl;
		rcvid = MsgReceive(receiveChid, message, sizeof(message), NULL);
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
		case Controller::MOTOR_FORWARD:
			openTimerRunning = true;
			std::cout << "Start open timer" << std::endl;
			pthread_create( &doorOpeningTimerThread, NULL, &RunDoorOpeningTimerThread, (void*)chids->hardwareFacadeChid );
			break;
		case Controller::MOTOR_REVERSE:
			closeTimerRunning = true;
			std::cout << "Start close timer" << std::endl;
			pthread_create( &doorClosingTimerThread, NULL, &RunDoorClosingTimerThread, (void*)chids->hardwareFacadeChid );
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
			if(openTimerRunning){
				pthread_kill(doorOpeningTimerThread, 0);
				openTimerRunning = false;
			}

			if(closeTimerRunning) {
				pthread_kill(doorClosingTimerThread, 0);
				closeTimerRunning = false;
			}
			m1.HandleEvent(e);
			break;
		}
	}

	return EXIT_SUCCESS;
}

void *RunHardwareFacade(void* args) {
	std::cout << "Hardware facade running..." << std::endl;

	Channels* chids = (Channels*)args;
	int sendChid = chids->inputScannerChid;
	int receiveChid = chids->hardwareFacadeChid;

	// connect to the channel we will send on
	int coid;
	coid = ConnectAttach(0, getpid(), sendChid, 0, 0);
	char rmsg[200];
	if (coid == -1) {
		std::cout << "Couldn't Connect :(" << std::endl;
		return 0;
	}

	char message[512];
	int rcvid;

	while(1){
		// check for message
		rcvid = MsgReceive(receiveChid, message, sizeof(message), NULL);
		std::cout << "GOT message in hardware facade " << message << std::endl;

		// send a response to wherever the message originally came from
		std::string response =  "Sending response";
		MsgReply(rcvid, 1, &response, sizeof(response));

		// forward to input scanner
		if(MsgSend(coid, message, strlen(message) + 1, rmsg, sizeof(rmsg)) == -1) {
			std::cout << "Could not send message :[ " << message << std::endl;
		}
	}

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

	std::cout << "Main running... " << std::endl;

	pthread_t inputScannerThread;
	pthread_t controllerThread;
	pthread_t hardwareFacadeThread;
	pthread_t keyboardScannerThread;

	int inputScannerChid = ChannelCreate(0);
	int controllerChid = ChannelCreate(0);
	int facadeChid = ChannelCreate(0);

	Channels channels = { inputScannerChid, controllerChid, facadeChid };

	pthread_create( &inputScannerThread, NULL, &RunInputScaner, (void*)&channels );
	pthread_create( &controllerThread, NULL, &RunController, (void*)&channels );
	pthread_create( &hardwareFacadeThread, NULL, &RunHardwareFacade, (void*)&channels);
	pthread_create( &keyboardScannerThread, NULL, &RunKeyboardScannerThread, (void*)&channels);

	pthread_join(inputScannerThread, NULL);
	pthread_join(controllerThread, NULL);
	pthread_join(hardwareFacadeThread, NULL);

	return EXIT_SUCCESS;
}
