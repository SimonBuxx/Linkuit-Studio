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
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mCurrentState);
    }
    else
    {
        return mCurrentState;
    }
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
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicAndGateCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentState = LogicState::LOW;
    mIsActive = false;
    emit StateChangedSignal();
}
