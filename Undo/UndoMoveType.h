#ifndef UNDOMOVETYPE_H
#define UNDOMOVETYPE_H

#include "UndoBaseType.h"
#include "../Components/IBaseComponent.h"

///
/// \brief The UndoMoveType class represents an undo action where objects have been moved in the scene
///
class UndoMoveType : public UndoBaseType
{
public:
    /// \brief Constructor for the UndoMoveType
    /// \param pMovedComponents: Components that have been moved
    /// \param pOffset: Offset from the original component positions
    UndoMoveType(std::vector<IBaseComponent*> &pMovedComponents, QPointF pOffset);

    /// \brief Constructor for the UndoMoveType
    /// \param pMovedComponents: Components that have been moved
    /// \param pAddedComponents: Components that have been added after moving
    /// \param pDeletedComponents: Components that have been deleted after moving
    /// \param pOffset: Offset from the original component positions
    UndoMoveType(std::vector<IBaseComponent*> &pMovedComponents, std::vector<IBaseComponent*> &pAddedComponents, std::vector<IBaseComponent*> &pDeletedComponents, QPointF pOffset);

    /// \brief Destructor for the UndoMoveType
    ~UndoMoveType(void);

    /// \brief Gets the type of this undo action
    /// \return An Undo::Type object
    Undo::Type Type(void) const override;

    /// \brief Getter for the moved components
    /// \return Vector containing pointers to the moved components
    const std::vector<IBaseComponent*>& MovedComponents(void) const;

    /// \brief Getter for the added components
    /// \return Vector containing pointers to the added components
    const std::vector<IBaseComponent*>& AddedComponents(void) const;

    /// \brief Getter for the deleted components
    /// \return Vector containing pointers to the deleted components
    const std::vector<IBaseComponent*>& DeletedComponents(void) const;

    /// \brief Getter for the offset by that the components have been moved
    /// \return The offset as a QPointF
    const QPointF Offset(void) const;

protected:
    std::vector<IBaseComponent*> mMovedComponents;
    std::vector<IBaseComponent*> mAddedComponents;
    std::vector<IBaseComponent*> mDeletedComponents;

    QPointF mOffset;
};

#endif // UNDOMOVETYPE_H
