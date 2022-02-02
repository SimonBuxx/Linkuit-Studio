#include "CoreLogic.h"
#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"

CoreLogic::CoreLogic(View &pView):
    mView(pView)
{
    ConnectToView();
}

void CoreLogic::ConnectToView()
{
    QObject::connect(this, &CoreLogic::ControlModeChangedSignal, &mView, &View::OnControlModeChanged);
}

void CoreLogic::EnterControlMode(ControlMode pMode)
{
    switch (pMode)
    {
        case ControlMode::EDIT:
        {
            mAddComponent = Component::NONE;
            break;
        }
        case ControlMode::ADD_AND_GATE:
        {
            mAddComponent = Component::AND_GATE;
            break;
        }
        case ControlMode::ADD_OR_GATE:
        {
            mAddComponent = Component::OR_GATE;
            break;
        }
        case ControlMode::ADD_XOR_GATE:
        {
            mAddComponent = Component::XOR_GATE;
            break;
        }
        default:
        {
            return;
        }
    }
    mControlMode = pMode;
    emit ControlModeChangedSignal(pMode);
}

QGraphicsItem* CoreLogic::GetItem()
{
#warning save reference to the component in core logic
    switch(mAddComponent)
    {
        case Component::AND_GATE:
        {
            return new AndGate(mAddInputCount, mAddDirection);
        }
        case Component::OR_GATE:
        {
            return new OrGate(mAddInputCount, mAddDirection);
        }
        case Component::XOR_GATE:
        {
            return new XorGate(mAddInputCount, mAddDirection);
        }
        default:
        {
            return nullptr;
        }
    }
}

bool CoreLogic::IsInAddMode()
{
#warning refactor
    return (mControlMode == ControlMode::ADD_AND_GATE || mControlMode == ControlMode::ADD_OR_GATE || mControlMode == ControlMode::ADD_XOR_GATE);
}

ControlMode CoreLogic::GetControlMode()
{
    return mControlMode;
}
