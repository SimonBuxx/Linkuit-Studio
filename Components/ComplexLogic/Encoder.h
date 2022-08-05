#ifndef ENCODER_H
#define ENCODER_H

#include "AbstractComplexLogic.h"

///
/// \brief The Encoder class represents a logic encoder
///
class Encoder : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for Encoder
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    /// \param pOutputCount: The amount of output digits for this encoder
    Encoder(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pOutputCount);

    /// \brief Copy constructor for Encoder
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    Encoder(const Encoder& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    Encoder(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const override;
};

#endif // ENCODER_H
