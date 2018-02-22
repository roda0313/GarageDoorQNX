/*
 * Machine.h
 *
 *  Created on: Feb 20, 2018
 *      Author: Daniel
 */

#ifndef MACHINE_H_
#define MACHINE_H_

#include <iostream>
#include "Common.h"

namespace Controller {

class Machine
{
  class State *current;
  public:
    Machine();
    ~Machine();

    void SetState(State *s)
    {
        current = s;
    }

    void HandleEvent(Events);
};

class State
{
  public:
    virtual void HandleEvent(Machine *m, Events e)
    {
        std::cout << "   default HandleEvent\n";
    }
};
}

#endif /* MACHINE_H_ */
