#ifndef UNDOBASETYPE_H
#define UNDOBASETYPE_H

#include "HelperStructures.h"

///
/// \brief The UndoBaseType class represents a user action that can be undone
///
class UndoBaseType
{
public:
    /// \brief Constructor for the UndoBaseType
    UndoBaseType();

    /// \brief Default destructor for the UndoBaseType
    virtual ~UndoBaseType() = default;

    /// \brief Gets the type of this undo action
    /// \return An Undo::Type object
    virtual Undo::Type Type(void) const = 0;
};

#endif // UNDOBASETYPE_H
