#include "LogicOrGateCell.h"
#include "HelperFunctions.h"

LogicOrGateCell::LogicOrGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1)
{}

void LogicOrGateCell::LogicFunction()
{
    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            if (mCurrentOutputStates[0] != LogicState::HIGH)
            {
                mNextOutputStates[0] = LogicState::HIGH;
                mStateChanged = true;
            }
            return;
        }
    }
    if (mCurrentOutputStates[0] != LogicState::LOW)
    {
        mNextOutputStates[0] = LogicState::LOW;
        mStateChanged = true;
    }
}

LogicState LogicOrGateCell::GetOutputState(uint32_t pOutput) const
{
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mCurrentOutputStates[0]);
    }
    else
    {
        return mCurrentOutputStates[0];
    }
}

void LogicOrGateCell::OnWakeUp()
{
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = true;
    mStateChanged = true;
}

void LogicOrGateCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = false;
    mStateChanged = true;
}
