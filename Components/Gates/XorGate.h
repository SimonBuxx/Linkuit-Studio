#ifndef XORGATE_H
#define XORGATE_H

#include "AbstractGate.h"

class XorGate : public AbstractGate
{
    Q_OBJECT
public:
    XorGate(uint8_t pInputCount, Direction pDirection);
    XorGate(const XorGate& pObj);

    virtual BaseComponent* CloneBaseComponent() const override;
};

#endif // XORGATE_H
