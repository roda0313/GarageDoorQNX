
#ifndef COMMON_H
#define COMMON_H

#include <sstream>
#include <string>

#define IO_PORT_SIZE 1
#define BASE 0x280
#define PORTA BASE+0x08
#define PORTB BASE+0x09
#define READ_WRITE_ADDRESS 0x28B

#define PIN0 0x01
#define PIN1 0x02
#define PIN2 0x04
#define PIN3 0x08
#define PIN4 0x10
#define PIN5 0x20
#define PIN6 0x40
#define PIN7 0x80

namespace Controller {
	const bool DEBUG_MODE = true;

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
