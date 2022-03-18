#include "LogicAndGateCell.h"

LogicAndGateCell::LogicAndGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1),
    mPreviousState(LogicState::LOW),
    mCurrentState(LogicState::LOW),
    mStateChanged(true)
{}

void LogicAndGateCell::LogicFunction()
{
    for (const auto& input : mInputStates)
    {
        if (input != LogicState::HIGH)
        {
            if (mPreviousState != LogicState::LOW)
            {
                mCurrentState = LogicState::LOW;
                mStateChanged = true;
            }
            return;
        }
    }
    if (mPreviousState != LogicState::HIGH)
    {
        mCurrentState = LogicState::HIGH;
        mStateChanged = true;
    }
}

LogicState LogicAndGateCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentState;
}

void LogicAndGateCell::OnSimulationAdvance()
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

void LogicAndGateCell::OnWakeUp()
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

void LogicAndGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentState = LogicState::LOW;
    emit StateChangedSignal();
}
