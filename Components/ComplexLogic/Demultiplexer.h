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
    /// \param pBitWidth: The amount of input digits for this demultiplexer
    Demultiplexer(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pBitWidth);

    /// \brief Copy constructor for Demultiplexer
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    Demultiplexer(const Demultiplexer& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    Demultiplexer(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

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
    uint8_t mBitWidth;
};

#endif // DEMULTIPLEXER_H
