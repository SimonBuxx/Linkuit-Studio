#include "LogicWireCell.h"
#include "CoreLogic.h"

LogicWireCell::LogicWireCell(const CoreLogic* pCoreLogic):
    LogicBaseCell(0, 0),
    mState(LogicState::LOW),
    mStateChanged(true)
{
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, this, &LogicWireCell::OnShutdown);
    QObject::connect(pCoreLogic, &CoreLogic::SimulationAdvanceSignal, this, &LogicWireCell::OnSimulationAdvance);
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
                mStateChanged = true;
                emit StateChangedSignal();
            }
            return;
        }
    }
    if (mState != LogicState::LOW)
    {
        mState = LogicState::LOW;
        mStateChanged = true;
        emit StateChangedSignal();
    }
}

void LogicWireCell::AppendOutput(std::shared_ptr<LogicBaseCell> pCell, uint32_t pInput)
{
    mOutputCells.push_back(std::make_pair(pCell, pInput));
}

void LogicWireCell::AddInputSlot()
{
    mInputStates.push_back(LogicState::LOW);
}

uint32_t LogicWireCell::GetInputSize(void) const
{
    return mInputStates.size();
}

void LogicWireCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        for (size_t i = 0; i < mOutputCells.size(); i++)
        {
            NotifySuccessor(i, mState);
        }
        mStateChanged = false;
    }
}

LogicState LogicWireCell::GetState() const
{
    return mState;
}

void LogicWireCell::OnShutdown()
{
    mState = LogicState::LOW;
    mOutputCells.clear();
    mInputStates.clear();
    mStateChanged = true;
    emit StateChangedSignal();
}
