#ifndef NOTGATE_H
#define NOTGATE_H

#include "AbstractGate.h"

class NotGate : public AbstractGate
{
    Q_OBJECT
public:
    NotGate(Direction pDirection);
    NotGate(const NotGate& pObj);

    virtual BaseComponent* CloneBaseComponent() const override;
};

#endif // NOTGATE_H
