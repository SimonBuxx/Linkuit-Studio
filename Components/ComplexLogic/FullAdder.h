#ifndef FULLADDER_H
#define FULLADDER_H

#include "AbstractComplexLogic.h"

///
/// \brief The FullAdder class represents a full-adder component
///
class FullAdder : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for FullAdder
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    FullAdder(const CoreLogic* pCoreLogic, Direction pDirection);

    /// \brief Copy constructor for FullAdder
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    FullAdder(const FullAdder& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    FullAdder(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    QJsonObject GetJson(void) const override;

    /// \brief Gets the minimum version compatible with this component
    /// \return the minimum version
    SwVersion GetMinVersion(void) const override;
};

#endif // FULLADDER_H
