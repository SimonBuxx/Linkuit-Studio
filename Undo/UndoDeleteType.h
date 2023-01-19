#ifndef UNDODELETETYPE_H
#define UNDODELETETYPE_H

#include "UndoBaseType.h"
#include "../Components/IBaseComponent.h"

///
/// \brief The UndoDeleteType class represents an undo action where objects where deleted
///
class UndoDeleteType : public UndoBaseType
{
public:
    /// \brief Constructor for the UndoDeleteType
    /// \param pDeletedComponents: Components that have been deleted
    UndoDeleteType(std::vector<IBaseComponent*> &pDeletedComponents);

    /// \brief Destructor for the UndoDeleteType
    ~UndoDeleteType(void) override;

    /// \brief Gets the type of this undo action
    /// \return An undo::Type object
    undo::Type Type(void) const override;

    /// \brief Getter for the deleted components
    /// \return Vector containing pointers to the deleted components
    const std::vector<IBaseComponent*>& Components(void) const;

protected:
    std::vector<IBaseComponent*> mComponents;
};

#endif // UNDODELETETYPE_H
