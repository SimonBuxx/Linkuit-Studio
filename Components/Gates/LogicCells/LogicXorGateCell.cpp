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
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mCurrentState);
        mPreviousState = mCurrentState;

        emit StateChangedSignal();
    }
}

void LogicXorGateCell::OnWakeUp()
{
    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mPreviousState = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    emit StateChangedSignal();
}

void LogicXorGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentState = LogicState::LOW;
    emit StateChangedSignal();
}
