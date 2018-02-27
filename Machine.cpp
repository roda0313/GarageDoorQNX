/*
 * Machine.cpp
 *
 *  Created on: Feb 20, 2018
 *      Author: Daniel
 */

#include "Machine.h"
#include "ClosedState.h"
#include <sys/neutrino.h>
#include <unistd.h>
#include <sstream>
#include <string.h>

namespace Controller {

Machine::Machine(int chid) {
	current = new ClosedState();
	m_chid = chid;

	m_coid = ConnectAttach(0, getpid(), m_chid, 0, 0);
	if (m_coid == -1) {
		std::cout << "Couldn't Connect :(" << std::endl;
		return;
	}
}

Machine::~Machine() {
	// TODO Auto-generated destructor stub
}

void Machine::HandleEvent(Events e) {
	current->HandleEvent(this, e);
}

void Machine::SendEvent(char e) {

	if(DEBUG_MODE)
		std::cout << "Attempting to send event from Machine: " << e << std::endl;

	char rmsg[200];
	if(MsgSend(m_coid, &e, strlen(&e) + 1, rmsg, sizeof(rmsg)) == -1) {
		std::cout << "Could not send message :[ " << e << std::endl;
	}
}

}
