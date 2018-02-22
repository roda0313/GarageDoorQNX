/*
 * ClosedState.cpp
 *
 *  Created on: Feb 20, 2018
 *      Author: Daniel
 */

#include "ClosedState.h"
#include "OpeningState.h";
#include <iostream>

namespace Controller {

ClosedState::ClosedState() {
	// TODO Auto-generated constructor stub

}

ClosedState::~ClosedState() {
	// TODO Auto-generated destructor stub
}

void ClosedState::HandleEvent(Machine *m, Events e){
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
