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
            mNextState = LogicState::HIGH;
            mStateChanged = true;
            emit StateChangedSignal();
            break;
        }
        case LogicState::HIGH:
        {
            mNextState = LogicState::LOW;
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

LogicState LogicNotGateCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentState;
}

void LogicNotGateCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mNextState);
        mCurrentState = mNextState;
        mStateChanged = false;
    }
}

void LogicNotGateCell::OnShutdown()
{
    mInputStates[0] = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    mNextState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
