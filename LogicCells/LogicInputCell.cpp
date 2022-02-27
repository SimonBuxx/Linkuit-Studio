#include "LogicInputCell.h"

LogicInputCell::LogicInputCell():
    LogicBaseCell(0, 1),
    mState(LogicState::LOW),
    mStateChanged(true)
{}

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
    mStateChanged = true;
    emit StateChangedSignal();
}

void LogicInputCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mState);
        mStateChanged = false;
    }
}

LogicState LogicInputCell::GetState() const
{
    return mState;
}

void LogicInputCell::OnShutdown()
{
    mState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
