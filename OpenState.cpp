/*
 * OpenState.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#include "OpenState.h"
#include "ClosingState.h"

namespace Controller {

OpenState::OpenState() {
	std::cout << "Creating OpenState object" << std::endl;
}

OpenState::~OpenState() {
	// TODO Auto-generated destructor stub
}

void OpenState::HandleEvent(Machine *m, Events e) {
	if(e == PUSH_BUTTON) {
		// handle the event then change state
		std::cout << "Got push button event, moving to Closing state" << std::endl;
		m->SetState(new ClosingState());
	}
	else {
		std::cout << "Unhandled event " << e << " in state OpenState" << std::endl;
		return; // dont destroy object, stay in state
	}

	delete this;
}

}
