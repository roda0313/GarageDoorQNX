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
	// TODO Auto-generated constructor stub

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
		std::cout << "Unhandled event " << e << " in state " << typeid(this).name() << std::endl;
	}

	delete this;
}

}
