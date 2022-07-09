#ifndef DEMULTIPLEXER_H
#define DEMULTIPLEXER_H

#include "AbstractComplexLogic.h"

///
/// \brief The Demultiplexer class represents a logic demultiplexer
///
class Demultiplexer : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for Demultiplexer
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    /// \param pDigitCount: The amount of input digits for this demultiplexer
    Demultiplexer(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pDigitCount);

    /// \brief Copy constructor for Demultiplexer
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    Demultiplexer(const Demultiplexer& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

#warning implement JSON conversion for this component
    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const override {};

protected:
    uint8_t mDigitCount;
};

#endif // DEMULTIPLEXER_H
