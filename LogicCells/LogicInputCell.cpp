#include "LogicInputCell.h"

LogicInputCell::LogicInputCell():
    LogicBaseCell(0, 1),
    mState(LogicState::LOW)
{

}

void LogicInputCell::LogicFunction()
{}

void LogicInputCell::ToggleState()
{
    if (mState == LogicState::LOW)
    {
        mState = LogicState::HIGH;
    }
    else
    {
        mState = LogicState::LOW;
    }
    NotifySuccessor(0, mState);
}

void LogicInputCell::SetState(LogicState pState)
{
    mState = pState;
    NotifySuccessor(0, mState);
}

LogicState LogicInputCell::GetState() const
{
    return mState;
}

void LogicInputCell::Shutdown()
{
    mState = LogicState::LOW;
}
