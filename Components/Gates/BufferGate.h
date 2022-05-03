#ifndef BUFFERGATE_H
#define BUFFERGATE_H

#include "AbstractGate.h"

///
/// \brief The BufferGate class represents a logic buffer gate
///
class BufferGate : public AbstractGate
{
    Q_OBJECT
public:
    /// \brief Constructor for the buffer gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the gate
    BufferGate(const CoreLogic* pCoreLogic, Direction pDirection);

    /// \brief Copy constructor for the buffer gate
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    BufferGate(const BufferGate& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new gate component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;
};

#endif // BUFFERGATE_H
