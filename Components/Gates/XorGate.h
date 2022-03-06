#ifndef XORGATE_H
#define XORGATE_H

#include "AbstractGate.h"

class XorGate : public AbstractGate
{
    Q_OBJECT
public:
    /// \brief Constructor for the XOR gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pInputCount: The number of inputs of the gate
    /// \param pDirection: The direction of the gate
    XorGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection);

    XorGate(const XorGate& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new gate component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // XORGATE_H
