/*
 * OpenStoppedState.h
 *
 *  Created on: Feb 22, 2018
 *      Author: Daniel
 */

#ifndef OPENSTOPPEDSTATE_H_
#define OPENSTOPPEDSTATE_H_

#include "Machine.h"

namespace Controller {

class OpenStoppedState {
public:
	OpenStoppedState();
	virtual ~OpenStoppedState();

	void HandleEvent(Machine *m, Events e);
};

}

#endif /* OPENSTOPPEDSTATE_H_ */
