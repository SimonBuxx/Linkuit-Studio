#ifndef UNDOADDTYPE_H
#define UNDOADDTYPE_H

#include "UndoBaseType.h"
#include "../Components/IBaseComponent.h"

///
/// \brief The UndoAddType class represents an undo action where objects where added and deleted
///
class UndoAddType : public UndoBaseType
{
public:
    /// \brief Constructor for the UndoAddType
    /// \param pAddedComponents: Components that have been added
    UndoAddType(std::vector<IBaseComponent*> &pAddedComponents);

    /// \brief Constructor for the UndoAddType
    /// \param pAddedComponents: Components that have been added
    /// \param pDeletedComponents: Components that have been deleted
    UndoAddType(std::vector<IBaseComponent*> &pAddedComponents, std::vector<IBaseComponent*> &pDeletedComponents);

    /// \brief Destructor for the UndoAddType
    ~UndoAddType(void) override;

    /// \brief Gets the type of this undo action
    /// \return An undo::Type object
    undo::Type Type(void) const override;

    /// \brief Getter for the added components
    /// \return Vector containing pointers to the added components
    const std::vector<IBaseComponent*>& AddedComponents(void) const;

    /// \brief Getter for the deleted components
    /// \return Vector containing pointers to the deleted components
    const std::vector<IBaseComponent*>& DeletedComponents(void) const;

protected:
    std::vector<IBaseComponent*> mAddedComponents;
    std::vector<IBaseComponent*> mDeletedComponents;
};

#endif // UNDOADDTYPE_H
