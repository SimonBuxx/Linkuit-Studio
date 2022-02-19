#ifndef NOTGATE_H
#define NOTGATE_H

#include "AbstractGate.h"

class NotGate : public AbstractGate
{
    Q_OBJECT
public:
    NotGate(const CoreLogic* pCoreLogic, Direction pDirection);
    NotGate(const NotGate& pObj, const CoreLogic* pCoreLogic);

    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // NOTGATE_H
