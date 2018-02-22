/*
 * ClosingState.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#include "ClosingState.h"

namespace Controller {

ClosingState::ClosingState() {
	// TODO Auto-generated constructor stub

}

ClosingState::~ClosingState() {
	// TODO Auto-generated destructor stub
}

void ClosingState::HandleEvent(Machine *m, Events e){
	if(e == PUSH_BUTTON) {
		// handle the event then change state
		std::cout << "Got push button event, moving to opening state" << std::endl;
		// m->SetState(new OpeningState());
		delete this;
	}
	else {
		std::cout << "Unhandled event " << e << " in state " << typeid(this).name() << std::endl;
	}
}

}
