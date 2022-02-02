#ifndef ANDGATE_H
#define ANDGATE_H

#include "AbstractGate.h"

class AndGate : public AbstractGate
{
    Q_OBJECT
public:
    AndGate(uint8_t pInputCount, Direction pDirection);
    AndGate(const AndGate& pObj);

    virtual BaseComponent* CloneBaseComponent() const override;
};

#endif // ANDGATE_H
