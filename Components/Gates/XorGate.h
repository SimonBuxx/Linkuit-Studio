#ifndef XORGATE_H
#define XORGATE_H

#include "AbstractGate.h"

///
/// \brief The AndGate class represents a logic XOR gate
///
class XorGate : public AbstractGate
{
    Q_OBJECT
public:
    /// \brief Constructor for the XOR gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pInputCount: The number of inputs of the gate
    /// \param pDirection: The direction of the gate
    XorGate(const CoreLogic* pCoreLogic, uint8_t pInputCount, Direction pDirection);

    /// \brief Copy constructor for the XOR gate
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    XorGate(const XorGate& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    XorGate(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

    /// \brief Clone function for the gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new gate component
    IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    QJsonObject GetJson(void) const override;

    /// \brief Gets the minimum version compatible with this component
    /// \return the minimum version
    SwVersion GetMinVersion(void) const override;
};

#endif // XORGATE_H
