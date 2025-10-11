#include "LogicClockCell.h"
#include "HelperFunctions.h"
#include <QJsonArray>

LogicClockCell::LogicClockCell():
    LogicBaseCell(0, 1),
    mToggleTicks(components::inputs::DEFAULT_CLOCK_TOGGLE_TICKS),
    mPulseTicks(components::inputs::DEFAULT_CLOCK_PULSE_TICKS),
    mTickCountdown(0),
    mPulseCountdown(0),
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
            if (mCurrentOutputStates[0] != LogicState::HIGH)
            {
                mNextOutputStates[0] = LogicState::HIGH;
                mStateChanged = true;
            }

            mPulseCountdown = mPulseTicks;
        }
        else
        {
            mNextOutputStates[0] = ((mCurrentOutputStates[0] == LogicState::HIGH) ? LogicState::LOW : LogicState::HIGH);
            mStateChanged = true;
        }

        mTickCountdown = mToggleTicks;
    }
    else if (mPulseCountdown == 0)
    {
        if (mCurrentOutputStates[0] != LogicState::LOW && mMode == ClockMode::PULSE)
        {
            mNextOutputStates[0] = LogicState::LOW;
            mStateChanged = true;
        }
        mPulseCountdown = mPulseTicks;
    }
}

LogicState LogicClockCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    if (mOutputInverted[0] && mIsActive)
    {
        return InvertState(mCurrentOutputStates[0]);
    }
    else
    {
        return mCurrentOutputStates[0];
    }
}

QJsonObject LogicClockCell::ExportCell() const
{
    QJsonObject obj;

    obj["UID"] = (int32_t) mUid;
    obj["Type"] = (int32_t) file::ComponentId::CLOCK;

    // Store connections
    QJsonArray outputCells;

    for (size_t output = 0; output < mOutputCells.size(); output++)
    {
        if (mOutputCells[output].first != nullptr) // Output connected
        {
            QJsonArray connection;

            connection.append((int32_t) mOutputCells[output].first->GetUid()); // UID
            connection.append((int32_t) mOutputCells[output].second); // Remote input
            connection.append((int32_t) output); // Local output

            outputCells.append(connection);
        }
    }

    obj["OutputCells"] = outputCells;
    obj["mode"] = static_cast<int32_t>(GetClockMode());
    obj["toggle"] = static_cast<int32_t>(GetToggleTicks());
    obj["pulse"] = static_cast<int32_t>(GetPulseTicks());

    obj["outinv"] = QJsonValue(GetOutputInversions()[0]);

    return obj;
}

void LogicClockCell::OnWakeUp()
{
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};

    mTickCountdown = mToggleTicks;
    mPulseCountdown = mPulseTicks;

    mIsActive = true;
    mStateChanged = true;
}

void LogicClockCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};

    if (!mIsInnerCell)
    {
        mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
        mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    }

    mIsActive = false;
    mStateChanged = true;
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

uint32_t LogicClockCell::GetToggleTicks() const
{
    return mToggleTicks;
}

uint32_t LogicClockCell::GetPulseTicks() const
{
   return  mPulseTicks;
}

ClockMode LogicClockCell::GetClockMode() const
{
    return mMode;
}

