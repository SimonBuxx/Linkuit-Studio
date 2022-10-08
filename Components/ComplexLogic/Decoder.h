#ifndef DECODER_H
#define DECODER_H

#include "AbstractComplexLogic.h"

///
/// \brief The Decoder class represents a logic decoder
///
class Decoder : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for Decoder
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    /// \param pInputCount: The amount of input digits for this decoder
    Decoder(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pInputCount);

    /// \brief Copy constructor for Decoder
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    Decoder(const Decoder& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    Decoder(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

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

#endif // DECODER_H
