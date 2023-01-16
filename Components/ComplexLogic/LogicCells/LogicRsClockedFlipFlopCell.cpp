#include "LogicRsClockedFlipFlopCell.h"

LogicRsClockedFlipFlopCell::LogicRsClockedFlipFlopCell():
    LogicBaseCell(3, 2),
    mOutputStates(2, LogicState::LOW),
    mPrevInputStates(2, LogicState::LOW),
    mStateChanged(true)
{}

void LogicRsClockedFlipFlopCell::LogicFunction()
{
    if (mPrevInputStates[1] == LogicState::LOW && mInputStates[1] == LogicState::HIGH) // rising edge
    {
        if (mInputStates[0] == LogicState::HIGH) // S high
        {
            if (mOutputStates[0] != LogicState::HIGH || mOutputStates[1] != LogicState::LOW)
            {
                mOutputStates[0] = LogicState::HIGH; // Q
                mOutputStates[1] = LogicState::LOW; // Not Q
                mStateChanged = true;
            }
        }
        if (mInputStates[2] == LogicState::HIGH) // R high
        {
            if (mOutputStates[0] != LogicState::LOW || mOutputStates[1] != LogicState::HIGH)
            {
                mOutputStates[0] = LogicState::LOW; // Q
                mOutputStates[1] = LogicState::HIGH; // Not Q
                mStateChanged = true;
            }
        }
    }

    if (mPrevInputStates[1] != mInputStates[1]) // Trigger repaint on every clock change
    {
        mStateChanged = true;
    }

    mPrevInputStates = mInputStates;
}

LogicState LogicRsClockedFlipFlopCell::GetOutputState(uint32_t pOutput) const
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

void LogicRsClockedFlipFlopCell::OnSimulationAdvance()
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

void LogicRsClockedFlipFlopCell::InputReady(uint32_t pInput, LogicState pState)
{
    if (mInputStates[pInput] != pState)
    {
        emit StateChangedSignal(); // to trigger immediate update of the component
    }
    LogicBaseCell::InputReady(pInput, pState);
}

void LogicRsClockedFlipFlopCell::OnWakeUp()
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

void LogicRsClockedFlipFlopCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
