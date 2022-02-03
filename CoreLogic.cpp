#include "CoreLogic.h"

#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"
#include "Components/Gates/NotGate.h"

#include "Undo/UndoAddType.h"
#include "Undo/UndoDeleteType.h"
#include "Undo/UndoMoveType.h"
#include "Undo/UndoConfigureType.h"

#include "HelperFunctions.h"

CoreLogic::CoreLogic(View &pView):
    mView(pView)
{
    ConnectToView();
    mView.Init();
}

void CoreLogic::ConnectToView()
{
    QObject::connect(this, &CoreLogic::ControlModeChangedSignal, &mView, &View::OnControlModeChanged);
    QObject::connect(this, &CoreLogic::ComponentTypeChangedSignal, &mView, &View::OnComponentTypeChanged);
}

void CoreLogic::EnterControlMode(ControlMode pMode)
{
    mControlMode = pMode;
    emit ControlModeChangedSignal(pMode);
    if (mControlMode == ControlMode::ADD)
    {
        emit ComponentTypeChangedSignal(mComponentType);
    }
}

void CoreLogic::EnterAddControlMode(ComponentType pComponentType)
{
    EnterControlMode(ControlMode::ADD);
    SetComponentType(pComponentType);
}

ComponentType CoreLogic::GetComponentType()
{
    return mComponentType;
}

void CoreLogic::SetComponentType(ComponentType pComponentType)
{
    mComponentType = pComponentType;
    emit ComponentTypeChangedSignal(mComponentType);
}

QGraphicsItem* CoreLogic::GetItem()
{
    QGraphicsItem* item;

    switch(mComponentType)
    {
        case ComponentType::AND_GATE:
        {
            item = new AndGate(mComponentInputCount, mComponentDirection);
            break;
        }
        case ComponentType::OR_GATE:
        {
            item = new OrGate(mComponentInputCount, mComponentDirection);
            break;
        }
        case ComponentType::XOR_GATE:
        {
            item = new XorGate(mComponentInputCount, mComponentDirection);
            break;
        }
        case ComponentType::NOT_GATE:
        {
            item = new NotGate(mComponentDirection);
            break;
        }
        default:
        {
            item = nullptr;
            break;
        }
    }

    return item;
}

ControlMode CoreLogic::GetControlMode()
{
    return mControlMode;
}

void CoreLogic::AddCurrentTypeComponent(QPointF pPosition)
{
    auto item = GetItem();
    Q_ASSERT(item);

    item->setPos(SnapToGrid(pPosition));

    if (mView.Scene()->collidingItems(item).empty())
    {
        mView.Scene()->addItem(item);

        auto addedComponents = std::vector<BaseComponent*>{static_cast<BaseComponent*>(item)};
        AppendUndo(new UndoAddType(addedComponents));
    }
    else
    {
        delete item;
    }
}

void CoreLogic::OnSelectedComponentsMoved(QPointF pOffset)
{
    if (pOffset.manhattanLength() <= 0)
    {
        return;
    }

    QList<QGraphicsItem*> movedGraphicsItems = mView.Scene()->selectedItems();
    std::vector<BaseComponent*> movedComponents{};
    for (const auto& comp : movedGraphicsItems)
    {
        movedComponents.push_back(static_cast<BaseComponent*>(comp));
    }

    if (movedComponents.size() > 0)
    {
        AppendUndo(new UndoMoveType(movedComponents, pOffset));
    }
}

void CoreLogic::CopySelectedComponents()
{
    QList<QGraphicsItem*> componentsToCopy = mView.Scene()->selectedItems();
    std::vector<BaseComponent*> addedComponents{};

    mView.Scene()->clearSelection();

    for (auto& orig : componentsToCopy)
    {
        // Create a copy of the original component
        BaseComponent* copy = static_cast<BaseComponent*>(orig)->CloneBaseComponent();
        Q_ASSERT(copy);

        // Paste the copied component one grid cell below and to the right
        copy->setPos(SnapToGrid(orig->pos() + QPointF(canvas::GRID_SIZE, canvas::GRID_SIZE)));
        copy->setSelected(true);
        mView.Scene()->addItem(copy);
        addedComponents.push_back(copy);
    }
    if (addedComponents.size() > 0)
    {
        AppendUndo(new UndoAddType(addedComponents));
    }
}

void CoreLogic::DeleteSelectedComponents()
{
    QList<QGraphicsItem*> componentsToDelete = mView.Scene()->selectedItems();
    std::vector<BaseComponent*> deletedComponents{};
    for (auto& comp : componentsToDelete)
    {
        mView.Scene()->removeItem(comp);
        deletedComponents.push_back(static_cast<BaseComponent*>(comp));
    }
    if (deletedComponents.size() > 0)
    {
        AppendUndo(new UndoDeleteType(deletedComponents));
    }
}

void CoreLogic::AppendUndo(UndoBaseType* pUndoObject)
{
    AppendToUndoQueue(pUndoObject, mUndoQueue);
    mRedoQueue.clear();
}

void CoreLogic::AppendToUndoQueue(UndoBaseType* pUndoObject, std::deque<UndoBaseType*> &pQueue)
{
    pQueue.push_back(pUndoObject);
    if (pQueue.size() > MAX_UNDO_STACK_SIZE)
    {
        delete pQueue.front();
        pQueue.pop_front();
    }
}

void CoreLogic::Undo()
{
    if (mUndoQueue.size() > 0)
    {
        UndoBaseType* undoObject = mUndoQueue.back();
        mUndoQueue.pop_back();
        Q_ASSERT(undoObject);

        switch (undoObject->Type())
        {
            case UndoType::ADD:
            {
                for (const auto& comp : static_cast<UndoAddType*>(undoObject)->Components())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->removeItem(comp);
                }
                AppendToUndoQueue(undoObject, mRedoQueue);
                break;
            }
            case UndoType::DEL:
            {
                for (const auto& comp : static_cast<UndoDeleteType*>(undoObject)->Components())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->addItem(comp);
                }
                AppendToUndoQueue(undoObject, mRedoQueue);
                break;
            }
            case UndoType::MOVE:
            {
                const auto undoMoveObject = static_cast<UndoMoveType*>(undoObject);
                for (const auto& comp : undoMoveObject->Components())
                {
                    Q_ASSERT(comp);
                    comp->moveBy(-undoMoveObject->Offset().x(), -undoMoveObject->Offset().y());
                }
                AppendToUndoQueue(undoObject, mRedoQueue);
                break;
            }
            case UndoType::CONFIGURE:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
    mView.Scene()->clearSelection();
}

void CoreLogic::Redo()
{
    if (mRedoQueue.size() > 0)
    {
        UndoBaseType* redoObject = mRedoQueue.back();
        mRedoQueue.pop_back();
        Q_ASSERT(redoObject);

        switch (redoObject->Type())
        {
            case UndoType::ADD:
            {
                for (const auto& comp : static_cast<UndoAddType*>(redoObject)->Components())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->addItem(comp);
                }
                AppendToUndoQueue(redoObject, mUndoQueue);
                break;
            }
            case UndoType::DEL:
            {
                for (const auto& comp : static_cast<UndoDeleteType*>(redoObject)->Components())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->removeItem(comp);
                }
                AppendToUndoQueue(redoObject, mUndoQueue);
                break;
            }
            case UndoType::MOVE:
            {
                const auto redoMoveObject = static_cast<UndoMoveType*>(redoObject);
                for (const auto& comp : redoMoveObject->Components())
                {
                    Q_ASSERT(comp);
                    comp->moveBy(redoMoveObject->Offset().x(), redoMoveObject->Offset().y());
                }
                AppendToUndoQueue(redoObject, mUndoQueue);
                break;
            }
            case UndoType::CONFIGURE:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
    mView.Scene()->clearSelection();
}
