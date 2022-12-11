#include "LogicJkMsFlipFlopCell.h"

LogicJkMsFlipFlopCell::LogicJkMsFlipFlopCell():
    LogicBaseCell(3, 2),
    mOutputStates(2, LogicState::LOW),
    mPrevInputStates(3, LogicState::LOW),
    mInternalState(LogicState::LOW),
    mStateChanged(true)
{}

void LogicJkMsFlipFlopCell::LogicFunction()
{
    if (mPrevInputStates[1] == LogicState::LOW && mInputStates[1] == LogicState::HIGH) // rising edge
    {
        // read in state
        if (mPrevInputStates[0] == LogicState::HIGH && mPrevInputStates[2] == LogicState::HIGH) // both => toggle
        {
            if (mInternalState == LogicState::LOW)
            {
                mInternalState = LogicState::HIGH;
            }
            else
            {
                mInternalState = LogicState::LOW;
            }
        }
        else if (mPrevInputStates[0] == LogicState::HIGH && mPrevInputStates[2] == LogicState::LOW) // J => pull up
        {
            mInternalState = LogicState::HIGH;
        }
        else if (mPrevInputStates[0] == LogicState::LOW && mPrevInputStates[2] == LogicState::HIGH) // K => pull down
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

LogicState LogicJkMsFlipFlopCell::GetOutputState(uint32_t pOutput) const
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

void LogicJkMsFlipFlopCell::OnSimulationAdvance()
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

void LogicJkMsFlipFlopCell::InputReady(uint32_t pInput, LogicState pState)
{
    if (mInputStates[pInput] != pState)
    {
        emit StateChangedSignal(); // to trigger immediate update of the component
    }
    LogicBaseCell::InputReady(pInput, pState);
}

void LogicJkMsFlipFlopCell::OnWakeUp()
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

void LogicJkMsFlipFlopCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mOutputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
