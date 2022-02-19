#ifndef ANDGATE_H
#define ANDGATE_H

#include "AbstractGate.h"

class AndGate : public AbstractGate
{
    Q_OBJECT
public:
    AndGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection);
    AndGate(const AndGate& pObj, const CoreLogic* pCoreLogic);

    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // ANDGATE_H
