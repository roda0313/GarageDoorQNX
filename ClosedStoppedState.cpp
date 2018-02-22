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
	// TODO Auto-generated constructor stub

}

ClosedStoppedState::~ClosedStoppedState() {
	// TODO Auto-generated destructor stub
}

void ClosedStoppedState::HandleEvent(Machine *m, Events e){
	if(e == PUSH_BUTTON) {
		// handle the event then change state
		std::cout << "Got push button event, moving to opening state" << std::endl;
		m->SetState(new OpeningState());
	}
	else {
		std::cout << "Unhandled event " << e << " in state " << typeid(this).name() << std::endl;
	}

	delete this;
}

}
