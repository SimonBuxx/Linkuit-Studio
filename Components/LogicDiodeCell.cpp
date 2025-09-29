#include "LogicDiodeCell.h"
#include "CoreLogic.h"

LogicDiodeCell::LogicDiodeCell(const CoreLogic* pCoreLogic):
    LogicBaseCell(1, 1) // Diodes always have exactly one input wire and one output wire
{
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal, this, &LogicDiodeCell::OnWakeUp);
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, this, &LogicDiodeCell::OnShutdown);
}

void LogicDiodeCell::SetInputState(uint32_t pInput, LogicState pState)
{
    LogicBaseCell::SetInputState(pInput, pState);

    LogicState newOutputState = LogicState::LOW;

    if (mCurrentOutputStates[0] != mInputStates[0])
    {
        newOutputState = mInputStates[0];

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

void LogicDiodeCell::OnCommitState()
{}

LogicState LogicDiodeCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentOutputStates[0];
}

void LogicDiodeCell::OnWakeUp()
{
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = true;
    mStateChanged = true;
}

void LogicDiodeCell::OnShutdown()
{
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>{LogicState::LOW};
    //mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mOutputInverted = std::vector<bool>{false};
    mInputInverted = std::vector<bool>{false};
    mIsActive = false;
    mStateChanged = true;
}
