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
    Machine(int chid);
    ~Machine();

    void SetState(State *s)
    {
        current = s;
    }

    void HandleEvent(Events);
    void SendEvent(char);

  private:
    int m_chid;
    int m_coid;
};

class State
{
  public:
    virtual void HandleEvent(Machine *m, Events e)
    {
        std::cout << "   default HandleEvent\n" << std::endl;
    }
};
}

#endif /* MACHINE_H_ */
