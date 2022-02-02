#include "CoreLogic.h"
#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"
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
        default:
        {
            item = nullptr;
            break;
        }
    }

    return item;
}

void CoreLogic::CopySelectedComponents()
{
    QList<QGraphicsItem*> componentsToCopy = mView.Scene()->selectedItems();
    mView.Scene()->clearSelection();
    for (auto& orig : componentsToCopy)
    {
        // Create a copy of the original component
        BaseComponent* copiedComponent = static_cast<BaseComponent*>(orig)->CloneBaseComponent();
        Q_ASSERT(copiedComponent);

        // Paste the copied component one grid cell below and to the right
        copiedComponent->setPos(SnapToGrid(orig->pos() + QPointF(canvas::GRID_SIZE, canvas::GRID_SIZE)));
        copiedComponent->setSelected(true);
        mView.Scene()->addItem(copiedComponent);
    }
}

ControlMode CoreLogic::GetControlMode()
{
    return mControlMode;
}

void CoreLogic::DeleteSelectedComponents()
{
#warning Push to undo stack
    QList<QGraphicsItem*> componentsToDelete = mView.Scene()->selectedItems();
    for (auto& comp : componentsToDelete)
    {
        mView.Scene()->removeItem(comp);
        delete comp; // Do NOT delete here if undo/redo implemented
    }
}

void CoreLogic::Undo()
{

}

void CoreLogic::Redo()
{

}
