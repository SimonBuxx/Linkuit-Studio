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
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mCurrentState);
    }
    else
    {
        return mCurrentState;
    }
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
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicXorGateCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mCurrentState = LogicState::LOW;
    mIsActive = false;
    emit StateChangedSignal();
}
