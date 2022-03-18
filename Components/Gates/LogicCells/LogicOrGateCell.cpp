#include "LogicOrGateCell.h"

LogicOrGateCell::LogicOrGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1),
    mPreviousState(LogicState::LOW),
    mCurrentState(LogicState::LOW),
    mStateChanged(true)
{}

void LogicOrGateCell::LogicFunction()
{
    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            if (mPreviousState != LogicState::HIGH)
            {
                mCurrentState = LogicState::HIGH;
                mStateChanged = true;
            }
            return;
        }
    }
    if (mPreviousState != LogicState::LOW)
    {
        mCurrentState = LogicState::LOW;
        mStateChanged = true;
    }
}

LogicState LogicOrGateCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentState;
}

void LogicOrGateCell::OnSimulationAdvance()
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

void LogicOrGateCell::OnWakeUp()
{
    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mPreviousState = LogicState::LOW;
    mCurrentState = LogicState::LOW;
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    emit StateChangedSignal();
}

void LogicOrGateCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
    mCurrentState = LogicState::LOW;
    emit StateChangedSignal();
}
