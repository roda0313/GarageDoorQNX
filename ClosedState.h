/*
 * ClosedState.h
 *
 *  Created on: Feb 20, 2018
 *      Author: Daniel
 */

#ifndef CLOSEDSTATE_H_
#define CLOSEDSTATE_H_

#include "Machine.h"

namespace Controller {

class ClosedState : public State {
public:
	ClosedState();
	virtual ~ClosedState();

	void HandleEvent(Machine *m, Events e);
};

}

#endif /* CLOSEDSTATE_H_ */
