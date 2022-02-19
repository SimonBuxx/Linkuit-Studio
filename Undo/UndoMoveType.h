#ifndef UNDOMOVETYPE_H
#define UNDOMOVETYPE_H

#include "UndoBaseType.h"
#include "../Components/BaseComponent.h"

class UndoMoveType : public UndoBaseType
{
public:
    UndoMoveType(std::vector<BaseComponent*> &pMovedComponents, QPointF pOffset);
    UndoMoveType(std::vector<BaseComponent*> &pMovedComponents, std::vector<BaseComponent*> &pAddedComponents, std::vector<BaseComponent*> &pDeletedComponents, QPointF pOffset);
    Undo::Type Type(void) const override;
    ~UndoMoveType(void);

    const std::vector<BaseComponent*>& MovedComponents(void) const;
    const std::vector<BaseComponent*>& AddedComponents(void) const;
    const std::vector<BaseComponent*>& DeletedComponents(void) const;
    const QPointF Offset(void) const;

protected:
    std::vector<BaseComponent*> mMovedComponents;
    std::vector<BaseComponent*> mAddedComponents;
    std::vector<BaseComponent*> mDeletedComponents;
    QPointF mOffset;
};

#endif // UNDOMOVETYPE_H
