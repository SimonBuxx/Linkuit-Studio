#ifndef ANDGATE_H
#define ANDGATE_H

#include "AbstractGate.h"

class AndGate : public AbstractGate
{
    Q_OBJECT
public:
    /// \brief Constructor for the AND gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pInputCount: The number of inputs of the gate
    /// \param pDirection: The direction of the gate
    AndGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection);

    AndGate(const AndGate& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new gate component
    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // ANDGATE_H
