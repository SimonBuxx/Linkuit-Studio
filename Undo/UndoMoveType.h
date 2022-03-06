#ifndef UNDOMOVETYPE_H
#define UNDOMOVETYPE_H

#include "UndoBaseType.h"
#include "../Components/IBaseComponent.h"

class UndoMoveType : public UndoBaseType
{
public:
    UndoMoveType(std::vector<IBaseComponent*> &pMovedComponents, QPointF pOffset);
    UndoMoveType(std::vector<IBaseComponent*> &pMovedComponents, std::vector<IBaseComponent*> &pAddedComponents, std::vector<IBaseComponent*> &pDeletedComponents, QPointF pOffset);
    Undo::Type Type(void) const override;
    ~UndoMoveType(void);

    const std::vector<IBaseComponent*>& MovedComponents(void) const;
    const std::vector<IBaseComponent*>& AddedComponents(void) const;
    const std::vector<IBaseComponent*>& DeletedComponents(void) const;
    const QPointF Offset(void) const;

protected:
    std::vector<IBaseComponent*> mMovedComponents;
    std::vector<IBaseComponent*> mAddedComponents;
    std::vector<IBaseComponent*> mDeletedComponents;
    QPointF mOffset;
};

#endif // UNDOMOVETYPE_H
