#ifndef CUSTOMLOGIC_H
#define CUSTOMLOGIC_H

#include "AbstractComplexLogic.h"
#include "CustomsLibrary.h"

///
/// \brief The CustomLogic class represents a custom logic component
///
class CustomLogic : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for CustomLogic
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    CustomLogic(const CoreLogic* pCoreLogic, Direction pDirection, const CustomsLibrary& pLibrary, const CircuitId& pCircuitId);

    /// \brief Copy constructor for CustomLogic
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    CustomLogic(const CustomLogic& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    CustomLogic(const CoreLogic* pCoreLogic, const QJsonObject& pJson, const CustomsLibrary& pLibrary);

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

protected:
    void ConfigureAppearance(const CircuitId& pCircuitId);

protected:
    CircuitId mCircuitId;

    const CustomsLibrary& mLibrary;
};

#endif // CUSTOMLOGIC_H
