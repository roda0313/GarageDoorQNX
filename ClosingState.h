/*
 * ClosingState.h
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#ifndef CLOSINGSTATE_H_
#define CLOSINGSTATE_H_

#include "Machine.h"

namespace Controller {

class ClosingState : public State {
public:
	ClosingState();
	virtual ~ClosingState();

	void HandleEvent(Machine *m, Events e);
};

}

#endif /* CLOSINGSTATE_H_ */
