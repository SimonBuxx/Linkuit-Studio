#ifndef NOTGATE_H
#define NOTGATE_H

#include "AbstractGate.h"

///
/// \brief The AndGate class represents a logic NOT gate
///
class NotGate : public AbstractGate
{
    Q_OBJECT
public:
    /// \brief Constructor for the NOT gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the gate
    NotGate(const CoreLogic* pCoreLogic, Direction pDirection);

    /// \brief Copy constructor for the NOT gate
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    NotGate(const NotGate& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new gate component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

#warning implement JSON conversion for this component
    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const override {};
};

#endif // NOTGATE_H
