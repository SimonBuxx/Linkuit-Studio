#include "LogicWireCell.h"
#include "CoreLogic.h"

LogicWireCell::LogicWireCell(const CoreLogic* pCoreLogic):
    LogicBaseCell(0, 0),
    mState(LogicState::LOW)
{
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal, this, &LogicWireCell::OnWakeUp);
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, this, &LogicWireCell::OnShutdown);
}

void LogicWireCell::LogicFunction()
{
    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            if (mState != LogicState::HIGH)
            {
                mState = LogicState::HIGH;
                emit StateChangedSignal();
            }
            return;
        }
    }
    if (mState != LogicState::LOW)
    {
        mState = LogicState::LOW;
        emit StateChangedSignal();
    }
}

void LogicWireCell::AppendOutput(std::shared_ptr<LogicBaseCell> pCell, uint32_t pInput)
{
    mOutputCells.push_back(std::make_pair(pCell, pInput));
    mOutputInverted.push_back(false);
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

void LogicWireCell::InputReady(uint32_t pInput, LogicState pState)
{
    if (mInputStates[pInput] != pState)
    {
        mInputStates[pInput] = pState;
        LogicFunction();

        for (size_t i = 0; i < mOutputCells.size(); i++)
        {
            NotifySuccessor(i, mState);
        }
    }
}

LogicState LogicWireCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mState;
}

void LogicWireCell::OnWakeUp()
{
    mState = LogicState::LOW;
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicWireCell::OnShutdown()
{
    mState = LogicState::LOW;
    mOutputCells.clear();
    mInputStates.clear();
    mOutputInverted.clear();
    mInputInverted.clear();
    mIsActive = false;
    emit StateChangedSignal();
}
