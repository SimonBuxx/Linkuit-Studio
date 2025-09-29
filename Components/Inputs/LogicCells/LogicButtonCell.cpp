#include "LogicButtonCell.h"
#include "Configuration.h"

LogicButtonCell::LogicButtonCell():
    LogicBaseCell(0, 1),
    mRemainingTicks(0)
{}

void LogicButtonCell::LogicFunction()
{
    if (mCurrentOutputStates[0] == LogicState::HIGH)
    {
        mRemainingTicks--;
        if (mRemainingTicks == 0)
        {
            mNextOutputStates[0] = LogicState::LOW;
            mStateChanged = true;
        }
    }
}

void LogicButtonCell::ButtonClick()
{
    if (mCurrentOutputStates[0] != LogicState::HIGH)
    {
        mRemainingTicks = components::inputs::BUTTON_TOGGLE_TICKS;
        mNextOutputStates[0] = LogicState::HIGH;
        mStateChanged = true;
    }
    else // Allows buttons to be disabled again, especially during pause
    {
        mRemainingTicks = 0;
        mNextOutputStates[0] = LogicState::LOW;
        mStateChanged = true;
    }

    // Keep states synced and trigger immediate repaint to make buttons responsive during pause
    if (mCurrentOutputStates[0] != mNextOutputStates[0])
    {
        mCurrentOutputStates[0] = mNextOutputStates[0];
        emit StateChangedSignal();
    }
}

LogicState LogicButtonCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentOutputStates[0];
}

void LogicButtonCell::OnWakeUp()
{
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = true;
    mStateChanged = true;
}

void LogicButtonCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = false;
    mStateChanged = true;
}
