#ifndef XORGATE_H
#define XORGATE_H

#include "AbstractGate.h"

class XorGate : public AbstractGate
{
    Q_OBJECT
public:
    XorGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection);
    XorGate(const XorGate& pObj, const CoreLogic* pCoreLogic);

    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // XORGATE_H
