#include "LogicOutputCell.h"

LogicOutputCell::LogicOutputCell():
    LogicBaseCell(1, 0),
    mState(LogicState::LOW)
{}

void LogicOutputCell::LogicFunction()
{
    if (mState != mInputStates[0])
    {
        mState = mInputStates[0];
        mStateChanged = true;
    }
}

LogicState LogicOutputCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mState;
}

void LogicOutputCell::OnWakeUp()
{
    mInputStates = std::vector<LogicState>{1, LogicState::LOW};
    mState = LogicState::LOW;
    mIsActive = true;
    mStateChanged = true;
}

void LogicOutputCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{1, LogicState::LOW};
    mState = LogicState::LOW;

    if (!mIsInnerCell)
    {
        mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    }

    mIsActive = false;
    mStateChanged = true;
}
