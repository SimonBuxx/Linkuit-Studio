#ifndef DFLIPFLOP_H
#define DFLIPFLOP_H

#include "AbstractComplexLogic.h"

class DFlipFlop : public AbstractComplexLogic
{
    Q_OBJECT
public:
    DFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection);

    DFlipFlop(const DFlipFlop& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // DFLIPFLOP_H
