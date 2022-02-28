#include "LogicOrGateCell.h"

LogicOrGateCell::LogicOrGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1),
    mCurrentState(LogicState::LOW),
    mNextState(LogicState::LOW),
    mStateChanged(true)
{}

void LogicOrGateCell::LogicFunction()
{
    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            if (mCurrentState != LogicState::HIGH)
            {
                mNextState = LogicState::HIGH;
                mStateChanged = true;
                emit StateChangedSignal();
            }
            return;
        }
    }
    if (mCurrentState != LogicState::LOW)
    {
        mNextState = LogicState::LOW;
        mStateChanged = true;
        emit StateChangedSignal();
    }
}

LogicState LogicOrGateCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentState;
}

void LogicOrGateCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mNextState);
        mCurrentState = mNextState;
        mStateChanged = false;
    }
}

void LogicOrGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentState = LogicState::LOW;
    mNextState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
