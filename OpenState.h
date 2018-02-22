/*
 * OpenState.h
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#ifndef OPENSTATE_H_
#define OPENSTATE_H_

#include "Machine.h"

namespace Controller {

class OpenState : public State{
public:
	OpenState();
	virtual ~OpenState();

	void HandleEvent(Machine *m, Events e);
};

}

#endif /* OPENSTATE_H_ */
