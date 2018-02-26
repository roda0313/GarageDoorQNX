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
}

Machine::~Machine() {
	// TODO Auto-generated destructor stub
}

void Machine::HandleEvent(Events e) {
	current->HandleEvent(this, e);
}

void Machine::SendEvent(Events e) {

	int coid;
	coid = ConnectAttach(0, getpid(), m_chid, 0, 0);
	char rmsg[200];
	if (coid == -1) {
		std::cout << "Couldn't Connect :(" << std::endl;
		return;
	}

	// convert event to string
	std::stringstream strs;
	strs << (int)e;
	std::string temp_str = strs.str();
	char const* pchar = temp_str.c_str();

	std::cout << "Attempting to send event from Machine: " << pchar << std::endl;

	if(MsgSend(coid, pchar, strlen(pchar) + 1, rmsg, sizeof(rmsg)) == -1) {
		std::cout << "Could not send message :[ " << pchar << std::endl;
	}
}

}
