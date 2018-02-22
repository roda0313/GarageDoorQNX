/*
 * ClosedStoppedState.h
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#ifndef CLOSEDSTOPPEDSTATE_H_
#define CLOSEDSTOPPEDSTATE_H_

#include "Machine.h"

namespace Controller {

class ClosedStoppedState {
public:
	ClosedStoppedState();
	virtual ~ClosedStoppedState();

	void HandleEvent(Machine *m, Events e);
};

}

#endif /* CLOSEDSTOPPEDSTATE_H_ */