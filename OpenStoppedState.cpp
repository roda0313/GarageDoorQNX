/*
 * OpenStoppedState.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#include "OpenStoppedState.h"
#include "ClosedState.h"

namespace Controller {

OpenStoppedState::OpenStoppedState() {
	// TODO Auto-generated constructor stub

}

OpenStoppedState::~OpenStoppedState() {
	// TODO Auto-generated destructor stub
}

void OpenStoppedState::HandleEvent(Machine *m, Events e) {
	if(e == PUSH_BUTTON) {
		// handle the event then change state
		std::cout << "Got push button event, moving to Closed state" << std::endl;
		m->SetState(new ClosedState());
	}
	else {
		std::cout << "Unhandled event " << e << " in state " << typeid(this).name() << std::endl;
	}

	delete this;
}

}
