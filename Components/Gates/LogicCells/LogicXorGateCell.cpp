#include "LogicXorGateCell.h"

LogicXorGateCell::LogicXorGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1),
    mPreviousState(LogicState::LOW),
    mCurrentState(LogicState::LOW),
    mStateChanged(true)
{}

void LogicXorGateCell::LogicFunction()
{
    bool oneHigh = false;
    mCurrentState = mPreviousState; // Keep current state if no change

    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            if (oneHigh)
            {
                if (mPreviousState != LogicState::LOW)
                {
                    mCurrentState = LogicState::LOW;
                    mStateChanged = true;
                    emit StateChangedSignal();
                }
                return;
            }
            oneHigh = true;
        }
    }

    if (mPreviousState != (oneHigh ? LogicState::HIGH : LogicState::LOW))
    {
        mCurrentState = oneHigh ? LogicState::HIGH : LogicState::LOW;
        mStateChanged = true;
        emit StateChangedSignal();
        return;
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
        NotifySuccessor(0, mCurrentState);
        mPreviousState = mCurrentState;
        mStateChanged = false;
    }
}

void LogicXorGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mPreviousState = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
