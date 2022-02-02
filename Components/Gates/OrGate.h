#ifndef ORGATE_H
#define ORGATE_H

#include "AbstractGate.h"

class OrGate : public AbstractGate
{
public:
    OrGate(uint8_t pInputCount, Direction pDirection);
};

#endif // ORGATE_H
