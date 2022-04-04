#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include "AbstractComplexLogic.h"

///
/// \brief The Multiplexer class represents a logic multiplexer
///
class Multiplexer : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for Multiplexer
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    /// \param pDigitCount: The amount of input digits for this multiplexer
    Multiplexer(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pDigitCount);

    /// \brief Copy constructor for Multiplexer
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    Multiplexer(const Multiplexer& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

protected:
    uint8_t mDigitCount;
};

#endif // MULTIPLEXER_H
