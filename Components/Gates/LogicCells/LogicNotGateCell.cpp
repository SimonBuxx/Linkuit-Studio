#include "LogicNotGateCell.h"

LogicNotGateCell::LogicNotGateCell():
    LogicBaseCell(1, 1),
    mPreviousState(LogicState::LOW),
    mCurrentState(LogicState::LOW),
    mStateChanged(true)
{}

void LogicNotGateCell::LogicFunction()
{
    switch (mInputStates[0])
    {
        case LogicState::LOW:
        {
            mCurrentState = LogicState::HIGH;
            mStateChanged = true;
            break;
        }
        case LogicState::HIGH:
        {
            mCurrentState = LogicState::LOW;
            mStateChanged = true;
            break;
        }
        default:
        {
            Q_ASSERT(false);
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
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mCurrentState);
        mPreviousState = mCurrentState;

        emit StateChangedSignal();
    }
}

void LogicNotGateCell::OnWakeUp()
{
    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mPreviousState = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    emit StateChangedSignal();
}

void LogicNotGateCell::OnShutdown()
{
    mInputStates[0] = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    emit StateChangedSignal();
}
