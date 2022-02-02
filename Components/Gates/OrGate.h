#ifndef ORGATE_H
#define ORGATE_H

#include "AbstractGate.h"

class OrGate : public AbstractGate
{
    Q_OBJECT
public:
    OrGate(uint8_t pInputCount, Direction pDirection);
    OrGate(const OrGate& pObj);

    virtual BaseComponent* CloneBaseComponent() const override;
};

#endif // ORGATE_H
