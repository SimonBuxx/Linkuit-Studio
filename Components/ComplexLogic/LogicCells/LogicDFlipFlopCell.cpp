#include "LogicDFlipFlopCell.h"

LogicDFlipFlopCell::LogicDFlipFlopCell():
    LogicBaseCell(2, 2),
    mPreviousState(2, LogicState::LOW),
    mCurrentState(2, LogicState::LOW),
    mPreviousClockState(LogicState::LOW),
    mStateChanged(true)
{}

#warning change D flip flop logic cell to not edge trigger on startup when > is high
void LogicDFlipFlopCell::LogicFunction()
{
    mCurrentState = mPreviousState; // Keep current state if no change

    if (mInputStates[1] == LogicState::HIGH && mPreviousClockState == LogicState::LOW) // Clock | positive edge
    {
        mCurrentState[0] = mInputStates[0]; // Q
        mCurrentState[1] = (mInputStates[0] == LogicState::HIGH) ? LogicState::LOW : LogicState::HIGH; // Not Q
        mStateChanged = true;
        emit StateChangedSignal();
    }

    mPreviousClockState = mInputStates[1];
}

LogicState LogicDFlipFlopCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput <= 1);
    return mCurrentState[pOutput];
}

void LogicDFlipFlopCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mCurrentState[0]);
        NotifySuccessor(1, mCurrentState[1]);
        mPreviousState = mCurrentState;
        mStateChanged = false;
    }
}

void LogicDFlipFlopCell::OnWakeUp()
{
    mCurrentState[0] = LogicState::LOW; // Q
    mCurrentState[1] = LogicState::HIGH; // Not Q
}

void LogicDFlipFlopCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mPreviousState = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentState = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mPreviousClockState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
