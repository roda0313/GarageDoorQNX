/*
 * ClosedStoppedState.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#include "ClosedStoppedState.h"
#include "OpeningState.h"

namespace Controller {

ClosedStoppedState::ClosedStoppedState() {
	std::cout << "Creating ClosedStoppedState object" << std::endl;
}

ClosedStoppedState::~ClosedStoppedState() {
	// TODO Auto-generated destructor stub
}

void ClosedStoppedState::HandleEvent(Machine *m, Events e){
	if(e == PUSH_BUTTON) {
		// handle the event then change state
		std::cout << "Got push button event, moving to opening state" << std::endl;
		m->SetState(new OpeningState(m));
	}
	else {
		std::cout << "Unhandled event " << e << " in state ClosedStoppedState" << std::endl;
		return; // dont destroy object, stay in state
	}

	delete this;
}

}
