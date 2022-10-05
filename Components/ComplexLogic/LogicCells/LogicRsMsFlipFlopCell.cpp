#include "LogicRsMsFlipFlopCell.h"

LogicRsMsFlipFlopCell::LogicRsMsFlipFlopCell():
    LogicBaseCell(3, 2),
    mOutputStates(2, LogicState::LOW),
    mPrevInputStates(3, LogicState::LOW),
    mInternalState(LogicState::LOW),
    mStateChanged(true)
{}

void LogicRsMsFlipFlopCell::LogicFunction()
{
    if (mPrevInputStates[1] == LogicState::LOW && mInputStates[1] == LogicState::HIGH) // rising edge
    {
        // read in state
        if (mInputStates[0] == LogicState::HIGH) // S high
        {
            mInternalState = LogicState::HIGH;
        }
        if (mInputStates[2] == LogicState::HIGH) // R high
        {
            mInternalState = LogicState::LOW;
        }
    }
    else if (mPrevInputStates[1] == LogicState::HIGH && mInputStates[1] == LogicState::LOW) // falling edge
    {
        mOutputStates[0] = mInternalState;
        mOutputStates[1] = ((mOutputStates[0] == LogicState::HIGH) ? LogicState::LOW : LogicState::HIGH);
    }

    if (mPrevInputStates[1] != mInputStates[1]) // Trigger repaint on every clock change
    {
        mStateChanged = true;
    }

    mPrevInputStates = mInputStates;
}

LogicState LogicRsMsFlipFlopCell::GetOutputState(uint32_t pOutput) const
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

void LogicRsMsFlipFlopCell::OnSimulationAdvance()
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

void LogicRsMsFlipFlopCell::InputReady(uint32_t pInput, LogicState pState)
{
    if (mInputStates[pInput] != pState)
    {
        emit StateChangedSignal(); // to trigger immediate update of the component
    }
    LogicBaseCell::InputReady(pInput, pState);
}

void LogicRsMsFlipFlopCell::OnWakeUp()
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

void LogicRsMsFlipFlopCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mOutputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
