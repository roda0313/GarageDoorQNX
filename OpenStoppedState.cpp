/*
 * OpenStoppedState.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#include "OpenStoppedState.h"
#include "ClosedState.h"
#include "ClosingState.h"

namespace Controller {

OpenStoppedState::OpenStoppedState() {
	std::cout << "Creating OpenStoppedState object" << std::endl;
}

OpenStoppedState::~OpenStoppedState() {
	// TODO Auto-generated destructor stub
}

void OpenStoppedState::HandleEvent(Machine *m, Events e) {
	if(e == PUSH_BUTTON) {
		// handle the event then change state
		std::cout << "Got push button event, moving to Closing state" << std::endl;
		m->SetState(new ClosingState());
	}
	else {
		std::cout << "Unhandled event " << e << " in state OpenStoppedState" << std::endl;
		return; // dont destroy object, stay in state
	}

	delete this;
}

}
