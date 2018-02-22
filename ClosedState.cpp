/*
 * ClosedState.cpp
 *
 *  Created on: Feb 20, 2018
 *      Author: Daniel
 */

#include "ClosedState.h"
#include <iostream>

namespace Controller {

ClosedState::ClosedState() {
	// TODO Auto-generated constructor stub

}

ClosedState::~ClosedState() {
	// TODO Auto-generated destructor stub
}

void ClosedState::HandleEvent(Machine *m, Events e){

}

void ClosedState::OnEntry() {
	std::cout << "ClosedState Enter" << std::endl;
}

}
