#include "LogicFullAdderCell.h"

LogicFullAdderCell::LogicFullAdderCell():
    LogicBaseCell(3, 2),
    mOutputStates(2, LogicState::LOW),
    mStateChanged(true)
{}

void LogicFullAdderCell::LogicFunction()
{
    if ((mInputStates[0] == LogicState::HIGH && mInputStates[1] == LogicState::LOW && mInputStates[2] == LogicState::LOW) // One input high
            || (mInputStates[0] == LogicState::LOW && mInputStates[1] == LogicState::HIGH && mInputStates[2] == LogicState::LOW)
            || (mInputStates[0] == LogicState::LOW && mInputStates[1] == LogicState::LOW && mInputStates[2] == LogicState::HIGH))
    {
        AssureState(mOutputStates[0], LogicState::HIGH);
        AssureState(mOutputStates[1], LogicState::LOW);
    }
    else if ((mInputStates[0] == LogicState::HIGH && mInputStates[1] == LogicState::HIGH && mInputStates[2] == LogicState::LOW) // Two inputs high
             || (mInputStates[0] == LogicState::HIGH && mInputStates[1] == LogicState::LOW && mInputStates[2] == LogicState::HIGH)
             || (mInputStates[0] == LogicState::LOW && mInputStates[1] == LogicState::HIGH && mInputStates[2] == LogicState::HIGH))
    {
        AssureState(mOutputStates[0], LogicState::LOW);
        AssureState(mOutputStates[1], LogicState::HIGH);
    }
    else if (mInputStates[0] == LogicState::HIGH && mInputStates[1] == LogicState::HIGH && mInputStates[2] == LogicState::HIGH) // All inputs high
    {
        AssureState(mOutputStates[0], LogicState::HIGH);
        AssureState(mOutputStates[1], LogicState::HIGH);
    }
    else // No input high
    {
        AssureState(mOutputStates[0], LogicState::LOW);
        AssureState(mOutputStates[1], LogicState::LOW);
    }
}

void LogicFullAdderCell::AssureState(LogicState &pSubject, LogicState pTargetState)
{
    if (pSubject != pTargetState)
    {
        pSubject = pTargetState;
        mStateChanged = true;
    }
}

LogicState LogicFullAdderCell::GetOutputState(uint32_t pOutput) const
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

void LogicFullAdderCell::OnSimulationAdvance()
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

void LogicFullAdderCell::OnWakeUp()
{
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mOutputStates[0] = LogicState::LOW;
    mOutputStates[1] = LogicState::LOW;
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicFullAdderCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
