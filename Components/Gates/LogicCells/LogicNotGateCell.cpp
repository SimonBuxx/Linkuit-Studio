#include "LogicNotGateCell.h"

LogicNotGateCell::LogicNotGateCell():
    LogicBaseCell(1, 1),
    mPreviousState(LogicState::LOW),
    mCurrentState(LogicState::LOW),
    mStateChanged(true)
{}

void LogicNotGateCell::LogicFunction()
{
    mCurrentState = mPreviousState; // Keep current state if no change

    switch (mInputStates[0])
    {
        case LogicState::LOW:
        {
            mCurrentState = LogicState::HIGH;
            mStateChanged = true;
            emit StateChangedSignal();
            break;
        }
        case LogicState::HIGH:
        {
            mCurrentState = LogicState::LOW;
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
        NotifySuccessor(0, mCurrentState);
        mPreviousState = mCurrentState;
        mStateChanged = false;
    }
}

void LogicNotGateCell::OnShutdown()
{
    mInputStates[0] = LogicState::LOW;
    mPreviousState = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
