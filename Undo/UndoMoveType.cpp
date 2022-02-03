#include "UndoMoveType.h"

UndoMoveType::UndoMoveType(std::vector<BaseComponent*> &pMovedComponents, QPointF pOffset):
    mComponents(pMovedComponents),
    mOffset(pOffset)
{}

UndoType UndoMoveType::Type() const
{
    return UndoType::MOVE;
}

const std::vector<BaseComponent*>& UndoMoveType::Components() const
{
    return mComponents;
}

const QPointF UndoMoveType::Offset() const
{
    return mOffset;
}

