#include "LogicXorGateCell.h"

LogicXorGateCell::LogicXorGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1),
    mCurrentState(LogicState::LOW),
    mNextState(LogicState::LOW),
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
                if (mCurrentState != LogicState::LOW)
                {
                    mNextState = LogicState::LOW;
                    mStateChanged = true;
                    emit StateChangedSignal();
                }
                return;
            }
            oneHigh = true;
        }
    }

    if (mCurrentState != (oneHigh ? LogicState::HIGH : LogicState::LOW))
    {
        mNextState = oneHigh ? LogicState::HIGH : LogicState::LOW;
        mStateChanged = true;
        emit StateChangedSignal();
    }
}

LogicState LogicXorGateCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentState;
}

void LogicXorGateCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mNextState);
        mCurrentState = mNextState;
        mStateChanged = false;
    }
}

void LogicXorGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentState = LogicState::LOW;
    mNextState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
