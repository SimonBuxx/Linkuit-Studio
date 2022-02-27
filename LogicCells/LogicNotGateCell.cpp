#include "LogicNotGateCell.h"

LogicNotGateCell::LogicNotGateCell():
    LogicBaseCell(1, 1),
    mStateChanged(true)
{}

void LogicNotGateCell::LogicFunction()
{
    switch (mInputStates[0])
    {
        case LogicState::LOW:
        {
            mOutputState = LogicState::HIGH;
            mStateChanged = true;
            emit StateChangedSignal();
            break;
        }
        case LogicState::HIGH:
        {
            mOutputState = LogicState::LOW;
            mStateChanged = true;
            emit StateChangedSignal();
            break;
        }
        default:
        {
            break;
        }
    }
}

void LogicNotGateCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mOutputState);
        mStateChanged = false;
    }
}

void LogicNotGateCell::OnShutdown()
{
    mInputStates[0] = LogicState::LOW;
    mOutputState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
