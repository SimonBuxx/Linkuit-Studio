#ifndef RSCLOCKEDFLIPFLOP_H
#define RSCLOCKEDFLIPFLOP_H

#include "AbstractComplexLogic.h"

///
/// \brief The RsClockedFlipFlop class represents a rising-edge-triggered clocked RS flip-flop
///
class RsClockedFlipFlop : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for RsClockedFlipFlop
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    RsClockedFlipFlop(const CoreLogic* pCoreLogic, Direction pDirection);

    /// \brief Copy constructor for RsClockedFlipFlop
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    RsClockedFlipFlop(const RsClockedFlipFlop& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    RsClockedFlipFlop(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const override;

    /// \brief Gets the minimum version compatible with this component
    /// \return the minimum version
    virtual SwVersion GetMinVersion(void) const override;
};

#endif // RSCLOCKEDFLIPFLOP_H
