/*
 * ClosingState.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#include "ClosingState.h"
#include "ClosedStoppedState.h"
#include "ClosedState.h"
#include "OpeningState.h"

namespace Controller {

ClosingState::ClosingState() {
	std::cout << "Creating ClosingState object" << std::endl;
}

ClosingState::~ClosingState() {
	// TODO Auto-generated destructor stub
}

void ClosingState::HandleEvent(Machine *m, Events e){
	if(e == PUSH_BUTTON) {
		// handle the event then change state
		std::cout << "Got push button event, moving to ClosedStoppedState state" << std::endl;
		m->SetState(new ClosedStoppedState());
	}
	else if(e == DOOR_CLOSED) {
		// handle the event then change state
		std::cout << "Got door closed event, moving to Closed state" << std::endl;
		m->SetState(new ClosedState());
	}
	else if(e == IR_TRIP) {
		// handle the event then change state
		std::cout << "Got IR Trip event, moving to Opening state" << std::endl;
		m->SetState(new OpeningState());
		m->SendEvent(EventToChar(MOTOR_FORWARD));
	}
	else if(e == OVERCURRENT) {
		// handle the event then change state
		std::cout << "Got over current event, moving to Opening state" << std::endl;
		m->SetState(new OpeningState());
	}
	else {
		std::cout << "Unhandled event " << e << " in state ClosingState" << std::endl;
		return; // dont destroy object, stay in state
	}

	delete this;
}

}
