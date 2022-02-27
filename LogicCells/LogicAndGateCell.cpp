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
            if (mOutputState != LogicState::LOW)
            {
                mOutputState = LogicState::LOW;
                mStateChanged = true;
                emit StateChangedSignal();
            }
            return;
        }
    }
    if (mOutputState != LogicState::HIGH)
    {
        mOutputState = LogicState::HIGH;
        mStateChanged = true;
        emit StateChangedSignal();
    }
}

void LogicAndGateCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mOutputState);
        mStateChanged = false;
    }
}

void LogicAndGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mOutputState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
