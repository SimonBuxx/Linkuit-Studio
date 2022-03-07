#include "LogicRsFlipFlopCell.h"

LogicRsFlipFlopCell::LogicRsFlipFlopCell():
    LogicBaseCell(2, 2),
    mPreviousState(2, LogicState::LOW),
    mCurrentState(2, LogicState::LOW),
    mStateChanged(true)
{}

void LogicRsFlipFlopCell::LogicFunction()
{
    mCurrentState = mPreviousState; // Keep current state if no change

    if (mInputStates[0] == LogicState::HIGH) // S high
    {
        if (mCurrentState[0] != LogicState::HIGH || mCurrentState[1] != LogicState::LOW)
        {
            mCurrentState[0] = LogicState::HIGH; // Q
            mCurrentState[1] = LogicState::LOW; // Not Q
            mStateChanged = true;
            emit StateChangedSignal();
        }
    }
    else if (mInputStates[1] == LogicState::HIGH) // R high
    {
        if (mCurrentState[0] != LogicState::LOW || mCurrentState[1] != LogicState::HIGH)
        {
            mCurrentState[0] = LogicState::LOW; // Q
            mCurrentState[1] = LogicState::HIGH; // Not Q
            mStateChanged = true;
            emit StateChangedSignal();
        }
    }
}

LogicState LogicRsFlipFlopCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput <= 1);
    return mCurrentState[pOutput];
}

void LogicRsFlipFlopCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mCurrentState[0]);
        NotifySuccessor(1, mCurrentState[1]);
        mPreviousState = mCurrentState;
        mStateChanged = false;
    }
}

void LogicRsFlipFlopCell::OnWakeUp()
{
    mCurrentState[0] = LogicState::LOW; // Q
    mCurrentState[1] = LogicState::HIGH; // Not Q
}

void LogicRsFlipFlopCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mPreviousState = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentState = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mStateChanged = true;
    emit StateChangedSignal();
}
