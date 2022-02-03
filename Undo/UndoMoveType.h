#ifndef UNDOMOVETYPE_H
#define UNDOMOVETYPE_H

#include "UndoBaseType.h"
#include "../Components/BaseComponent.h"

class UndoMoveType : public UndoBaseType
{
public:
    UndoMoveType(std::vector<BaseComponent*> &pMovedComponents, QPointF pOffset);
    UndoType Type(void) const override;

    const std::vector<BaseComponent*>& Components(void) const;
    const QPointF Offset(void) const;

protected:
    std::vector<BaseComponent*> mComponents;
    QPointF mOffset;
};

#endif // UNDOMOVETYPE_H
