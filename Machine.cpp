/*
 * Machine.cpp
 *
 *  Created on: Feb 20, 2018
 *      Author: Daniel
 */

#include "Machine.h"
#include "ClosedState.h"

namespace Controller {

Machine::Machine() {
	current = new ClosedState();
}

Machine::~Machine() {
	// TODO Auto-generated destructor stub
}

void Machine::HandleEvent(Events e) {
	current->HandleEvent(this, e);
}

}
