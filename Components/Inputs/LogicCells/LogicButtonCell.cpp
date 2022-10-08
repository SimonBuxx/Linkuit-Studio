#include "LogicButtonCell.h"
#include "Configuration.h"

LogicButtonCell::LogicButtonCell():
    LogicBaseCell(0, 1),
    mState(LogicState::LOW),
    mRemainingTicks(0),
    mStateChanged(true)
{}

void LogicButtonCell::LogicFunction()
{
    if (mState == LogicState::HIGH)
    {
        mRemainingTicks--;
        if (mRemainingTicks == 0)
        {
            mState = LogicState::LOW;
            mStateChanged = true;
            emit StateChangedSignal();
        }
    }
}

void LogicButtonCell::ButtonClick()
{
    if (mState != LogicState::HIGH)
    {
        mRemainingTicks = components::inputs::BUTTON_TOGGLE_TICKS;
        mState = LogicState::HIGH;
        NotifySuccessor(0, mState);
        emit StateChangedSignal();
    }
}

LogicState LogicButtonCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mState;
}

void LogicButtonCell::OnSimulationAdvance()
{
    LogicFunction();

    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mState);

        emit StateChangedSignal();
    }
}

void LogicButtonCell::OnWakeUp()
{
    mState = LogicState::LOW;
    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicButtonCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mState = LogicState::LOW;
    mIsActive = false;
    emit StateChangedSignal();
}
