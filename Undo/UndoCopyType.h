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
    UndoCopyType(std::vector<IBaseComponent*> &pMovedComponents, std::vector<IBaseComponent*> &pAddedComponents, std::vector<IBaseComponent*> &pDeletedComponents, QPointF pOffset);

    /// \brief Destructor for the UndoCopyType
    ~UndoCopyType(void);

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

#warning missing documentation
    bool IsCompleted(void) const;

    void AppendAddedComponents(std::vector<IBaseComponent*>& pComponents);
    void AppendMovedComponents(std::vector<IBaseComponent*>& pComponents);
    void AppendDeletedComponents(std::vector<IBaseComponent*>& pComponents);
    void SetOffset(QPointF pOffset);
    void SetCompleted(bool pIsCompleted);

protected:
    std::vector<IBaseComponent*> mMovedComponents;
    std::vector<IBaseComponent*> mAddedComponents;
    std::vector<IBaseComponent*> mDeletedComponents;

    QPointF mOffset;

    bool mIsCompleted;
};

#endif // UNDOCOPYTYPE_H
