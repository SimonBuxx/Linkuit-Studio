#include "LogicDFlipFlopCell.h"
#include <QDebug>

LogicDFlipFlopCell::LogicDFlipFlopCell():
    LogicBaseCell(2, 2),
    mOutputStates(2, LogicState::LOW),
    mPrevInputStates(2, LogicState::LOW),
    mStateChanged(true)
{}

void LogicDFlipFlopCell::LogicFunction()
{
    if (mPrevInputStates[1] == LogicState::LOW && mInputStates[1] == LogicState::HIGH)
    {
        mOutputStates[0] = mPrevInputStates[0];
        mOutputStates[1] = ((mPrevInputStates[0] == LogicState::HIGH) ? LogicState::LOW : LogicState::HIGH);

        mStateChanged = true;
    }

    mPrevInputStates = mInputStates;
}

LogicState LogicDFlipFlopCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput <= 1);
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mOutputStates[pOutput]);
    }
    else
    {
        return mOutputStates[pOutput];
    }
}

void LogicDFlipFlopCell::OnSimulationAdvance()
{
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mOutputStates[0]);
        NotifySuccessor(1, mOutputStates[1]);

        emit StateChangedSignal();
    }
}

void LogicDFlipFlopCell::OnWakeUp()
{   
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mPrevInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    mOutputStates[0] = LogicState::LOW;
    mOutputStates[1] = LogicState::HIGH;
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicDFlipFlopCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
