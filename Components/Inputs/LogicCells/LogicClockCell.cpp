#include "LogicClockCell.h"

LogicClockCell::LogicClockCell():
    LogicBaseCell(0, 1),
    mState(LogicState::LOW),
    mTickCountdown(0),
    mPulseCountdown(0),
    mStateChanged(true)
{}

void LogicClockCell::LogicFunction()
{
    mTickCountdown--;
    mPulseCountdown--;
    if (mTickCountdown == 0)
    {
        if (components::inputs::CLOCK_ENABLE_PULSE)
        {
            if (mState != LogicState::HIGH)
            {
                mState = LogicState::HIGH;
                mStateChanged = true;
                emit StateChangedSignal();
            }

            mPulseCountdown = components::inputs::CLOCK_PULSE_TICKS;
        }
        else
        {
            mState = ((mState == LogicState::HIGH) ? LogicState::LOW : LogicState::HIGH);
            mStateChanged = true;
            emit StateChangedSignal();
        }

        mTickCountdown = components::inputs::CLOCK_TOGGLE_TICKS;
    }
    else if (mPulseCountdown == 0)
    {
        if (mState != LogicState::LOW)
        {
            mState = LogicState::LOW;
            mStateChanged = true;
            emit StateChangedSignal();
        }
        mPulseCountdown = components::inputs::CLOCK_PULSE_TICKS;
    }
}

LogicState LogicClockCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    if (mOutputInverted[0] && mIsActive)
    {
        return InvertState(mState);
    }
    else
    {
        return mState;
    }
}

void LogicClockCell::OnSimulationAdvance()
{
    LogicFunction();

    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mState);

        emit StateChangedSignal();
    }
}

void LogicClockCell::OnWakeUp()
{
    mState = LogicState::LOW;
    mNextUpdateTime = UpdateTime::NOW;

    mTickCountdown = components::inputs::CLOCK_TOGGLE_TICKS;
    mPulseCountdown = components::inputs::CLOCK_PULSE_TICKS;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicClockCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mState = LogicState::LOW;
    mIsActive = false;
    emit StateChangedSignal();
}
