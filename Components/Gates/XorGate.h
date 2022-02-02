#ifndef XORGATE_H
#define XORGATE_H

#include "AbstractGate.h"

class XorGate : public AbstractGate
{
public:
    XorGate(uint8_t pInputCount, Direction pDirection);
};

#endif // XORGATE_H
