#include "LogicAndGateCell.h"

LogicAndGateCell::LogicAndGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1),
    mStateChanged(true)
{}

void LogicAndGateCell::LogicFunction()
{
    for (const auto& input : mInputStates)
    {
        if (input != LogicState::HIGH)
        {
            if (mCurrentState != LogicState::LOW)
            {
                mNextState = LogicState::LOW;
                mStateChanged = true;
                emit StateChangedSignal();
            }
            return;
        }
    }
    if (mCurrentState != LogicState::HIGH)
    {
        mNextState = LogicState::HIGH;
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
        NotifySuccessor(0, mNextState);
        mCurrentState = mNextState;
        mStateChanged = false;
    }
}

void LogicAndGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentState = LogicState::LOW;
    mNextState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
