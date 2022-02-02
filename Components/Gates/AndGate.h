#ifndef ANDGATE_H
#define ANDGATE_H

#include "AbstractGate.h"

class AndGate : public AbstractGate
{
public:
    AndGate(uint8_t pInputCount, Direction pDirection);
};

#endif // ANDGATE_H
