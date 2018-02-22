/*
 * OpeningState.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#include "OpeningState.h"

namespace Controller {

OpeningState::OpeningState() {
	// TODO Auto-generated constructor stub

}

OpeningState::~OpeningState() {
	// TODO Auto-generated destructor stub
}

void OpeningState::HandleEvent(Machine *m, Events e){
	if(e == PUSH_BUTTON) {
		// handle the event then change state
		// m->SetState(new OpeningState());
		delete this;
	}
	else {
		std::cout << "Unhandled event " << e << " in state " << typeid(this).name() << std::endl;
	}
}

}
