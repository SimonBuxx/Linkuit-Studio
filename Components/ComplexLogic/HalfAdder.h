#ifndef HALFADDER_H
#define HALFADDER_H

#include "AbstractComplexLogic.h"

///
/// \brief The HalfAdder class represents a half-adder component
///
class HalfAdder : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for HalfAdder
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    HalfAdder(const CoreLogic* pCoreLogic, Direction pDirection);

    /// \brief Copy constructor for HalfAdder
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    HalfAdder(const HalfAdder& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    HalfAdder(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const override;
};

#endif // HALFADDER_H
