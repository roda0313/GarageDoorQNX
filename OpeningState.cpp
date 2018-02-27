/*
 * OpeningState.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#include "OpeningState.h"
#include "OpenStoppedState.h"
#include "OpenState.h"

namespace Controller {

OpeningState::OpeningState(Machine *m) {
	std::cout << "Creating OpeningState object" << std::endl;
	m->SendEvent(EventToChar(MOTOR_FORWARD));
}

OpeningState::~OpeningState() {
	// TODO Auto-generated destructor stub
}

void OpeningState::HandleEvent(Machine *m, Events e){
	if(e == PUSH_BUTTON) {
		// handle the event then change state
		std::cout << "Got push button event, moving to OpenStopped state" << std::endl;
		m->SetState(new OpenStoppedState());
	}
	else if(e == DOOR_OPENED) {
		// handle the event then change state
		std::cout << "Got door opened event, moving to OpenState state" << std::endl;
		m->SetState(new OpenState());
	}
	else if(e == OVERCURRENT) {
		// handle the event then change state
		std::cout << "Got over current event, moving to OpenStoppedState state" << std::endl;
		m->SetState(new OpenStoppedState());
	}
	else {
		std::cout << "Unhandled event " << e << " in state OpeningState" << std::endl;
		return; // dont destroy object, stay in state
	}

	delete this;
}

}
