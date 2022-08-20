#include "UndoCopyType.h"

UndoCopyType::UndoCopyType(std::vector<IBaseComponent*> &pAddedComponents):
    mAddedComponents(pAddedComponents),
    mIsCompleted(false)
{}

UndoCopyType::UndoCopyType(std::vector<IBaseComponent*> &pMovedComponents,
                           std::vector<IBaseComponent*> &pAddedComponents,
                           std::vector<IBaseComponent*> &pDeletedComponents,
                           QPointF pOffset):
    mMovedComponents(pMovedComponents),
    mAddedComponents(pAddedComponents),
    mDeletedComponents(pDeletedComponents),
    mOffset(pOffset),
    mIsCompleted(false)
{}

UndoCopyType::~UndoCopyType()
{
    for (auto& comp : mDeletedComponents)
    {
        delete comp;
    }
}

undo::Type UndoCopyType::Type() const
{
    return undo::Type::COPY;
}

const std::vector<IBaseComponent*>& UndoCopyType::MovedComponents() const
{
    return mMovedComponents;
}

const std::vector<IBaseComponent*>& UndoCopyType::AddedComponents() const
{
    return mAddedComponents;
}

const std::vector<IBaseComponent*>& UndoCopyType::DeletedComponents() const
{
    return mDeletedComponents;
}

const QPointF UndoCopyType::Offset() const
{
    return mOffset;
}

bool UndoCopyType::IsCompleted() const
{
    return mIsCompleted;
}

void UndoCopyType::AppendMovedComponents(std::vector<IBaseComponent*>& pComponents)
{
    Q_ASSERT(!mIsCompleted);
    mMovedComponents.insert(mMovedComponents.end(), pComponents.begin(), pComponents.end());
}

void UndoCopyType::AppendAddedComponents(std::vector<IBaseComponent*>& pComponents)
{
    Q_ASSERT(!mIsCompleted);
    mAddedComponents.insert(mAddedComponents.end(), pComponents.begin(), pComponents.end());
}

void UndoCopyType::AppendDeletedComponents(std::vector<IBaseComponent*>& pComponents)
{
    Q_ASSERT(!mIsCompleted);
    mDeletedComponents.insert(mDeletedComponents.end(), pComponents.begin(), pComponents.end());
}

void UndoCopyType::SetOffset(const QPointF& pOffset)
{
    Q_ASSERT(!mIsCompleted);
    mOffset = pOffset;
}

void UndoCopyType::MarkCompleted()
{
    Q_ASSERT(!mIsCompleted);
    mIsCompleted = true;
}
