/*
 * OpeningState.h
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#ifndef OPENINGSTATE_H_
#define OPENINGSTATE_H_

#include "Machine.h"

namespace Controller {

class OpeningState : public State {
public:
	OpeningState();
	virtual ~OpeningState();

	void HandleEvent(Machine *m, Events e);
};

}

#endif /* OPENINGSTATE_H_ */
