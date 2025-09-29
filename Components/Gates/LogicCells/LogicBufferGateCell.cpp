#include "LogicBufferGateCell.h"
#include "HelperFunctions.h"

LogicBufferGateCell::LogicBufferGateCell():
    LogicBaseCell(1, 1)
{}

void LogicBufferGateCell::LogicFunction()
{
    if (mCurrentOutputStates[0] != mInputStates[0])
    {
        mNextOutputStates[0] = mInputStates[0];
        mStateChanged = true;
    }
}

LogicState LogicBufferGateCell::GetOutputState(uint32_t pOutput) const
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

void LogicBufferGateCell::OnWakeUp()
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

void LogicBufferGateCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = false;
    mStateChanged = true;
}
