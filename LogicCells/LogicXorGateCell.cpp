#include "LogicXorGateCell.h"

LogicXorGateCell::LogicXorGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1),
    mStateChanged(true)
{}

void LogicXorGateCell::LogicFunction()
{
    bool oneHigh = false;
    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            if (oneHigh)
            {
                if (mOutputState != LogicState::LOW)
                {
                    mOutputState = LogicState::LOW;
                    mStateChanged = true;
                    emit StateChangedSignal();
                }
                return;
            }
            oneHigh = true;
        }
    }

    if (mOutputState != (oneHigh ? LogicState::HIGH : LogicState::LOW))
    {
        mOutputState = oneHigh ? LogicState::HIGH : LogicState::LOW;
        mStateChanged = true;
        emit StateChangedSignal();
    }
}

void LogicXorGateCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mOutputState);
        mStateChanged = false;
    }
}

void LogicXorGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mOutputState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
