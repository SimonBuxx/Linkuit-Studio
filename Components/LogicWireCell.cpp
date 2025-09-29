#include "LogicWireCell.h"

LogicWireCell::LogicWireCell():
    LogicBaseCell(0, 1)
{}

void LogicWireCell::SetInputState(uint32_t pInput, LogicState pState)
{
    LogicBaseCell::SetInputState(pInput, pState);

    LogicState newOutputState = LogicState::LOW;

    for (const auto& inputState : mInputStates)
    {
        if (inputState == LogicState::HIGH)
        {
            newOutputState = LogicState::HIGH;
            break;
        }
    }

    if (mCurrentOutputStates[0] != newOutputState)
    {
        emit StateChangedSignal();

        mCurrentOutputStates[0] = newOutputState;
        mNextOutputStates[0] = newOutputState;

        for (const auto& outputConnection : mOutputCells)
        {
            if (outputConnection.first != nullptr)
            {
                outputConnection.first->SetInputState(outputConnection.second, newOutputState);
            }
        }
    }
}

void LogicWireCell::OnCommitState()
{}

void LogicWireCell::AppendOutput(const std::shared_ptr<LogicBaseCell>& pLogicCell, uint32_t pInput)
{
    mOutputCells.push_back(std::make_pair(pLogicCell, pInput));
    mOutputInverted.push_back(false);
    pLogicCell->SetConnected(pInput);
}

void LogicWireCell::AddInputSlot()
{
    mInputStates.push_back(LogicState::LOW);
    mInputInverted.push_back(false);
}

uint32_t LogicWireCell::GetInputSize(void) const
{
    return mInputStates.size();
}

LogicState LogicWireCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentOutputStates[0];
}

void LogicWireCell::OnWakeUp()
{
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = true;
    mStateChanged = true;
}

void LogicWireCell::OnShutdown()
{
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mInputConnected.clear();
    mOutputCells.clear();
    mInputStates.clear();
    mOutputInverted.clear();
    mInputInverted.clear();
    mIsActive = false;
    mStateChanged = true;
}
