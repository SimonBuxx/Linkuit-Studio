#include "LogicOrGateCell.h"

LogicOrGateCell::LogicOrGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1),
    mStateChanged(true)
{}

void LogicOrGateCell::LogicFunction()
{
    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            if (mOutputState != LogicState::HIGH)
            {
                mOutputState = LogicState::HIGH;
                mStateChanged = true;
                emit StateChangedSignal();
            }
            return;
        }
    }
    if (mOutputState != LogicState::LOW)
    {
        mOutputState = LogicState::LOW;
        mStateChanged = true;
        emit StateChangedSignal();
    }
}

void LogicOrGateCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mOutputState);
        mStateChanged = false;
    }
}

void LogicOrGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mOutputState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
