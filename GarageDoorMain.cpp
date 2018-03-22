#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <unistd.h>
#include <string.h>
#include <hw/inout.h>

#include <stdint.h>
#include <sys/mman.h>

#include "Machine.h"

const bool DEBUG_MODE = Controller::DEBUG_MODE;

struct ThreadArgs {
	int inputScannerChid;
	int controllerChid;
	int hardwareFacadeChid;
	uintptr_t portAHandle;
	uintptr_t portBHandle;
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
	if (DEBUG_MODE)
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
	if (DEBUG_MODE)
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
	if (DEBUG_MODE)
		std::cout << "Keyboard Scanner running..." << std::endl;

	ThreadArgs* threadArgs = (ThreadArgs*)args;
	int chid = threadArgs->inputScannerChid;
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
		if (DEBUG_MODE)
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
			std::cout << "Shutting down keyboard scanner..." << std::endl;
			return EXIT_SUCCESS;
		}
		default:
			std::cout << "Invalid input " << inp << std::endl;
		}
	}

	return EXIT_SUCCESS;
}

void *RunHardwareScannerThread(void* args){
	if (DEBUG_MODE)
		std::cout << "Hardware Scanner running..." << std::endl;

	ThreadArgs* threadArgs = (ThreadArgs*)args;
	uintptr_t portAHandle = threadArgs->portAHandle;
	uintptr_t portBHandle = threadArgs->portBHandle;

	// reset board to default state
	out8(portAHandle, 0x00);
	out8(portAHandle, PIN3);

	int chid = threadArgs->inputScannerChid;
	int coid;
	coid = ConnectAttach(0, getpid(), chid, 0, 0);
	char rmsg[200];
	if (coid == -1) {
		std::cout << "Couldn't Connect :(" << std::endl;
		return 0;
	}

	while(1)
	{
		uint8_t inp = in8(portBHandle);
		if (DEBUG_MODE)
			std::cout << "GOT " << static_cast<int>(inp) << std::endl;

//		// motor up
//		out8(portAHandle, (PIN0 | PIN3));
//
//		sleep(11);
//
//		out8(portAHandle, (0x00 | PIN3));

		if((inp & PIN0) == PIN0) {
			char message = EventToChar(Controller::SHUTDOWN); // ? whatever this is
			if(MsgSend(coid, &message, strlen(&message) + 1, rmsg, sizeof(rmsg)) == -1) {
				std::cout << "Could not send message :[ " << inp << std::endl;
			}
		}
		else if((inp & PIN1) == PIN1) {

		}
		else if((inp & PIN2) == PIN2) {

		}
		else if((inp & PIN3) == PIN3) {

		}
		else if((inp & PIN4) == PIN4) {

		}
		else if((inp & PIN5) == PIN5) {

		}
		else if((inp & PIN6) == PIN6) {

		}
		else if((inp & PIN7) == PIN7) {

		}
		else {
			std::cout << "Invalid input " << static_cast<int>(inp) << std::endl;
		}

		usleep(900);
	}

	return EXIT_SUCCESS;
}

void *RunInputScaner(void* args) {
	if (DEBUG_MODE)
		std::cout << "Input Scanner running..." << std::endl;

	ThreadArgs* threadArgs = (ThreadArgs*)args;
	int sendChid = threadArgs->controllerChid;
	int receiveChid = threadArgs->inputScannerChid;

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
		if (DEBUG_MODE)
			std::cout << "GOT message in input scanner " << message << std::endl;

		// send a response to wherever the message originally came from
		std::string response =  "Sending response";
		MsgReply(rcvid, 1, &response, sizeof(response));

		// forward to controller
		if(MsgSend(coid, message, strlen(message) + 1, rmsg, sizeof(rmsg)) == -1) {
			std::cout << "Could not send message :[ " << message << std::endl;
		}

		if(CharToEvent(message[0]) == Controller::SHUTDOWN){
			std::cout <<"Shutting down input scanner..." << std::endl;
			break;
		}
	}

	return EXIT_SUCCESS;

}

void *RunController(void* args) {
	ThreadArgs* threadArgs = (ThreadArgs*)args;
	int receiveChid = threadArgs->controllerChid;
	int sendChid = threadArgs->hardwareFacadeChid;

	Controller::Machine m1 = Controller::Machine(sendChid);

	pthread_t doorOpeningTimerThread;
	pthread_t doorClosingTimerThread;
	bool openTimerRunning = false;
	bool closeTimerRunning = false;

	if (DEBUG_MODE)
		std::cout << "Controller running..." << std::endl;

	char message[512];
	int rcvid;

	while(1){
		// check for message
		if (DEBUG_MODE)
			std::cout << "Now waiting in controller for a message" << std::endl;
		rcvid = MsgReceive(receiveChid, message, sizeof(message), NULL);
		if (DEBUG_MODE)
			std::cout << "GOT message in controller " << message << std::endl;

		// get the event
		Controller::Events e = CharToEvent(message[0]);

		// send a response
		strcpy(message, "Sending response");
		MsgReply(rcvid, 1, message, sizeof(message));

		// check for timers, kill any that are running if required
		// if either timer is running, kill it
		if((openTimerRunning && e != Controller::DOOR_OPENED) || e == Controller::SHUTDOWN){
			pthread_cancel(doorOpeningTimerThread);
			openTimerRunning = false;
		}

		if((closeTimerRunning && e != Controller::DOOR_CLOSED) || e == Controller::SHUTDOWN) {
			pthread_cancel(doorClosingTimerThread);
			closeTimerRunning = false;
		}

		switch(e) {
		case Controller::SHUTDOWN:
			std::cout << "Shutting down controller..." << std::endl;
			// make sure hardware facade gets shutdown signal
			m1.SendEvent(EventToChar(Controller::SHUTDOWN));
			return EXIT_SUCCESS;
		case Controller::MOTOR_FORWARD:
			openTimerRunning = true;
			std::cout << "Start open timer" << std::endl;
			pthread_create( &doorOpeningTimerThread, NULL, &RunDoorOpeningTimerThread, (void*)threadArgs->hardwareFacadeChid );
			break;
		case Controller::MOTOR_REVERSE:
			closeTimerRunning = true;
			std::cout << "Start close timer" << std::endl;
			pthread_create( &doorClosingTimerThread, NULL, &RunDoorClosingTimerThread, (void*)threadArgs->hardwareFacadeChid );
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
			m1.HandleEvent(e);
			break;
		}
	}

	return EXIT_SUCCESS;
}

void *RunHardwareFacade(void* args) {
	if (DEBUG_MODE)
		std::cout << "Hardware facade running..." << std::endl;

	ThreadArgs* threadArgs = (ThreadArgs*)args;
	int sendChid = threadArgs->inputScannerChid;
	int receiveChid = threadArgs->hardwareFacadeChid;

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
		if (DEBUG_MODE)
			std::cout << "GOT message in hardware facade " << message << std::endl;

		// send a response to wherever the message originally came from
		std::string response =  "Sending response";
		MsgReply(rcvid, 1, &response, sizeof(response));

		// the hardware facade is the last thing to shutdown so we dont need to send the message before closing
		if(CharToEvent(message[0]) == Controller::SHUTDOWN){
			std::cout <<"Shutting down hardware facade..." << std::endl;
			break;
		}

		// forward to input scanner
		if(MsgSend(coid, message, strlen(message) + 1, rmsg, sizeof(rmsg)) == -1) {
			std::cout << "Could not send message :[ " << message << std::endl;
		}
	}

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

	if (DEBUG_MODE)
		std::cout << "Main running... " << std::endl;

	if ( ThreadCtl(_NTO_TCTL_IO, NULL) == -1)
	{
		std::perror("Failed to get I/O access permission");
		return 1;
	}

	// use port 0x28B (base + 11 offset) to set if port a/b is in or out
	uintptr_t ctrlHandle = mmap_device_io(IO_PORT_SIZE, READ_WRITE_ADDRESS);
	if(ctrlHandle == MAP_DEVICE_FAILED) {
		std::perror("Failed to map control register");
		return 0;
	}

	// set direction of ports in/out
	out8(ctrlHandle, PIN1);

	// outputs from the controller to the FPGA
	uintptr_t portAHandle = mmap_device_io(IO_PORT_SIZE, PORTA);
	if(portAHandle == MAP_DEVICE_FAILED) {
		std::perror("Failed to map control register");
		return 0;
	}

	// input to the controller (purple box)
	uintptr_t portBHandle = mmap_device_io(IO_PORT_SIZE, PORTB);
	if(portBHandle == MAP_DEVICE_FAILED) {
		std::perror("Failed to map control register");
		return 0;
	}

	pthread_t inputScannerThread;
	pthread_t controllerThread;
	pthread_t hardwareFacadeThread;
//	pthread_t keyboardScannerThread;
	pthread_t hardwareScannerThread;

	int inputScannerChid = ChannelCreate(0);
	int controllerChid = ChannelCreate(0);
	int facadeChid = ChannelCreate(0);

	ThreadArgs args = { inputScannerChid, controllerChid, facadeChid, portAHandle, portBHandle };

	pthread_create( &inputScannerThread, NULL, &RunInputScaner, (void*)&args );
	pthread_create( &controllerThread, NULL, &RunController, (void*)&args );
	pthread_create( &hardwareFacadeThread, NULL, &RunHardwareFacade, (void*)&args);
//	pthread_create( &keyboardScannerThread, NULL, &RunKeyboardScannerThread, (void*)&args);
	pthread_create( &hardwareScannerThread, NULL, &RunHardwareScannerThread, (void*)&args);

	pthread_join(inputScannerThread, NULL);
	pthread_join(controllerThread, NULL);
	pthread_join(hardwareFacadeThread, NULL);
//	pthread_join(keyboardScannerThread, NULL);
	pthread_join(hardwareScannerThread, NULL);

	return EXIT_SUCCESS;
}
