#ifndef RSFLIPFLOP_H
#define RSFLIPFLOP_H

#include "AbstractComplexLogic.h"

class RsFlipFlop : public AbstractComplexLogic
{
public:
    RsFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection);

    RsFlipFlop(const RsFlipFlop& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // RSFLIPFLOP_H
