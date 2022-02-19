#ifndef ORGATE_H
#define ORGATE_H

#include "AbstractGate.h"

class OrGate : public AbstractGate
{
    Q_OBJECT
public:
    OrGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection);
    OrGate(const OrGate& pObj, const CoreLogic* pCoreLogic);

    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // ORGATE_H
