#ifndef NOTGATE_H
#define NOTGATE_H

#include "AbstractGate.h"

class NotGate : public AbstractGate
{
    Q_OBJECT
public:
    /// \brief Constructor for the NOT gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the gate
    NotGate(const CoreLogic* pCoreLogic, Direction pDirection);

    NotGate(const NotGate& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new gate component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // NOTGATE_H
