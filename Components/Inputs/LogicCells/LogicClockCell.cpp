#include "LogicClockCell.h"

LogicClockCell::LogicClockCell():
    LogicBaseCell(0, 1),
    mState(LogicState::LOW),
    mToggleTicks(components::inputs::DEFAULT_CLOCK_TOGGLE_TICKS),
    mPulseTicks(components::inputs::DEFAULT_CLOCK_PULSE_TICKS),
    mTickCountdown(0),
    mPulseCountdown(0),
    mStateChanged(true),
    mMode(components::inputs::DEFAULT_CLOCK_MODE)
{}

void LogicClockCell::LogicFunction()
{
    mTickCountdown--;
    mPulseCountdown--;
    if (mTickCountdown == 0)
    {
        if (mMode == ClockMode::PULSE)
        {
            if (mState != LogicState::HIGH)
            {
                mState = LogicState::HIGH;
                mStateChanged = true;
                emit StateChangedSignal();
            }

            mPulseCountdown = mPulseTicks;
        }
        else
        {
            mState = ((mState == LogicState::HIGH) ? LogicState::LOW : LogicState::HIGH);
            mStateChanged = true;
            emit StateChangedSignal();
        }

        mTickCountdown = mToggleTicks;
    }
    else if (mPulseCountdown == 0)
    {
        if (mState != LogicState::LOW)
        {
            mState = LogicState::LOW;
            mStateChanged = true;
            emit StateChangedSignal();
        }
        mPulseCountdown = mPulseTicks;
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

    mTickCountdown = mToggleTicks;
    mPulseCountdown = mPulseTicks;

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

void LogicClockCell::SetToggleTicks(uint32_t pTicks)
{
    mToggleTicks = pTicks;
}

void LogicClockCell::SetPulseTicks(uint32_t pTicks)
{
    mPulseTicks = pTicks;
}

void LogicClockCell::SetClockMode(ClockMode pMode)
{
    mMode = pMode;
}

uint32_t LogicClockCell::GetToggleTicks()
{
    return mToggleTicks;
}

uint32_t LogicClockCell::GetPulseTicks()
{
   return  mPulseTicks;
}

ClockMode LogicClockCell::GetClockMode()
{
    return mMode;
}

