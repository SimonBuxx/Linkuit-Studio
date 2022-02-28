#include "LogicAndGateCell.h"

LogicAndGateCell::LogicAndGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1),
    mPreviousState(LogicState::LOW),
    mCurrentState(LogicState::LOW),
    mStateChanged(true)
{}

void LogicAndGateCell::LogicFunction()
{
    mCurrentState = mPreviousState; // Keep current state if no change

    for (const auto& input : mInputStates)
    {
        if (input != LogicState::HIGH)
        {
            if (mPreviousState != LogicState::LOW)
            {
                mCurrentState = LogicState::LOW;
                mStateChanged = true;
                emit StateChangedSignal();
            }
            return;
        }
    }
    if (mPreviousState != LogicState::HIGH)
    {
        mCurrentState = LogicState::HIGH;
        mStateChanged = true;
        emit StateChangedSignal();
    }
}

LogicState LogicAndGateCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentState;
}

void LogicAndGateCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mCurrentState);
        mPreviousState = mCurrentState;
        mStateChanged = false;
    }
}

void LogicAndGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mPreviousState = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
