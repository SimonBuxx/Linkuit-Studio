#ifndef RSFLIPFLOP_H
#define RSFLIPFLOP_H

#include "AbstractComplexLogic.h"

///
/// \brief The RsFlipFlop class represents an unclocked level-triggered RS flip-flop
///
class RsFlipFlop : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for RsFlipFlop
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    RsFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection);

    /// \brief Copy constructor for RsFlipFlop
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    RsFlipFlop(const RsFlipFlop& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

#warning implement JSON conversion for this component
    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const override {};
};

#endif // RSFLIPFLOP_H
