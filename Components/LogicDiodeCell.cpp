#include "LogicDiodeCell.h"
#include "CoreLogic.h"

LogicDiodeCell::LogicDiodeCell(const CoreLogic* pCoreLogic):
    LogicBaseCell(1, 1), // Diodes always have exactly one input wire and one output wire
    mState(LogicState::LOW)
{
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal, this, &LogicDiodeCell::OnWakeUp);
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, this, &LogicDiodeCell::OnShutdown);
}

void LogicDiodeCell::LogicFunction()
{
    if (mState != mInputStates[0])
    {
        mState = mInputStates[0];
        emit StateChangedSignal();
    }
}

void LogicDiodeCell::InputReady(uint32_t pInput, LogicState pState)
{
    Q_ASSERT(mInputStates.size() > pInput);

    if (mInputStates[pInput] != pState)
    {
        mInputStates[pInput] = pState;
        LogicFunction();

        NotifySuccessor(0, mState);
    }
}

LogicState LogicDiodeCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mState;
}

void LogicDiodeCell::OnWakeUp()
{
    mState = LogicState::LOW;
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicDiodeCell::OnShutdown()
{
    mState = LogicState::LOW;
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>{LogicState::LOW};
    mOutputInverted = std::vector<bool>{false};
    mInputInverted = std::vector<bool>{false};
    mIsActive = false;
    emit StateChangedSignal();
}
