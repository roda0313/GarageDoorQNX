
#ifndef COMMON_H
#define COMMON_H

#include <sstream>
#include <string>

#define IO_PORT_SIZE 1
#define CTRL_ADDRESS 0x37A

namespace Controller {
	const bool DEBUG_MODE = false;

	enum Events {
		PUSH_BUTTON = 0,
		IR_TRIP,
		OVERCURRENT,
		DOOR_OPENED,
		DOOR_CLOSED,
		IR_ON,
		MOTOR_FORWARD, // door opening
		MOTOR_REVERSE, // door closing
		SHUTDOWN,
	};

	static char EventToChar(Events e){
		int event = (int)e;

		std::ostringstream stm ;
		stm << event ;
		std::string ret = stm.str() ;
		return ret[0];
	}
}

#endif // COMMON_H
