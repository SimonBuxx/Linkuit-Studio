#include "LogicDemultiplexerCell.h"

LogicDemultiplexerCell::LogicDemultiplexerCell(uint8_t pDigitCount):
    LogicBaseCell(pDigitCount + 1, std::pow(2, pDigitCount)),
    mOutputStates(std::pow(2, pDigitCount), LogicState::LOW),
    mStateChanged(true),
    mDigitCount(pDigitCount),
    mPreviousOutput(0)
{}

void LogicDemultiplexerCell::LogicFunction()
{    
    uint32_t output = 0;

    for (size_t i = 0; i < mDigitCount; i++)
    {
        output += (mInputStates[i] == LogicState::HIGH) ? std::pow(2, i) : 0;
    }

    mStateChanged |= AssureStateIf(output != mPreviousOutput, mOutputStates[mPreviousOutput], LogicState::LOW);
    mStateChanged |= AssureState(mOutputStates[output], mInputStates[mDigitCount]);
    mPreviousOutput = output;
}

LogicState LogicDemultiplexerCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput < mOutputStates.size());
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mOutputStates[pOutput]);
    }
    else
    {
        return mOutputStates[pOutput];
    }
}

void LogicDemultiplexerCell::OnSimulationAdvance()
{
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        for (size_t i = 0; i < mOutputStates.size(); i++)
        {
            NotifySuccessor(i, mOutputStates[i]);
        }

        emit StateChangedSignal();
    }
}

void LogicDemultiplexerCell::OnWakeUp()
{   
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mNextUpdateTime = UpdateTime::NOW;

    mPreviousOutput = 0;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicDemultiplexerCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
