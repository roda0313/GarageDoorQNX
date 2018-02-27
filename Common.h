
#ifndef COMMON_H
#define COMMON_H

#include <sstream>
#include <string>

namespace Controller {
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
