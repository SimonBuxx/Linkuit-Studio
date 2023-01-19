#ifndef UNDOCOPYTYPE_H
#define UNDOCOPYTYPE_H

#include "UndoBaseType.h"
#include "../Components/IBaseComponent.h"

///
/// \brief The UndoCopyType class represents an undo action where objects where copied
///
class UndoCopyType : public UndoBaseType
{
public:
    /// \brief Constructor for the UndoCopyType
    /// \param pAddedComponents: Components that have been added after copying
    UndoCopyType(std::vector<IBaseComponent*> &pAddedComponents);

    /// \brief Constructor for the UndoCopyType
    /// \param pMovedComponents: Components that have been moved
    /// \param pAddedComponents: Components that have been added
    /// \param pDeletedComponents: Components that have been deleted
    /// \param pOffset: Offset from the original component positions
    UndoCopyType(std::vector<IBaseComponent*> &pMovedComponents,
                 std::vector<IBaseComponent*> &pAddedComponents,
                 std::vector<IBaseComponent*> &pDeletedComponents,
                 QPointF pOffset);

    /// \brief Destructor for the UndoCopyType
    ~UndoCopyType(void) override;

    /// \brief Gets the type of this undo action
    /// \return An undo::Type object
    undo::Type Type(void) const override;

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

    /// \brief Returns true, if this undo action is completed and has not been aborted
    /// \return True, if the undo action has been completed
    bool IsCompleted(void) const;

    /// \brief Appends the given components to the vector of added components
    /// \param pComponents: The components to append
    void AppendAddedComponents(std::vector<IBaseComponent*>& pComponents);

    /// \brief Appends the given components to the vector of moved components
    /// \param pComponents: The components to append
    void AppendMovedComponents(std::vector<IBaseComponent*>& pComponents);

    /// \brief Appends the given components to the vector of deleted components
    /// \param pComponents: The components to append
    void AppendDeletedComponents(std::vector<IBaseComponent*>& pComponents);

    /// \brief Setter for the offset by which the components have been moved after inserting
    /// \param pOffset: The offset as a QPointF
    void SetOffset(const QPointF& pOffset);

    /// \brief Marks this undo action as completed, meaning it has not been aborted
    /// An undo action that has been completed may not be set to incomplete again.
    void MarkCompleted(void);

protected:
    std::vector<IBaseComponent*> mMovedComponents;
    std::vector<IBaseComponent*> mAddedComponents;
    std::vector<IBaseComponent*> mDeletedComponents;

    QPointF mOffset; // Offset between point of insertion and point of action completion

    bool mIsCompleted;
};

#endif // UNDOCOPYTYPE_H
