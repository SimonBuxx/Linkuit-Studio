#include "LogicBaseCell.h"

#include <QThread>

LogicBaseCell::LogicBaseCell(uint32_t pInputs, uint32_t pOutputs):
    mInputStates{pInputs, LogicState::LOW},
    mOutputCells{pOutputs, std::make_pair(nullptr, 0)}
{}

void LogicBaseCell::ConnectOutput(std::shared_ptr<LogicBaseCell> pCell, uint32_t pInput, uint32_t pOutput)
{
    mOutputCells[pOutput] = std::make_pair(pCell, pInput);
}

void LogicBaseCell::LogicFunction()
{}

void LogicBaseCell::OnShutdown()
{}

void LogicBaseCell::OnSimulationAdvance()
{}

void LogicBaseCell::NotifySuccessor(uint32_t pOutput, LogicState pState) const
{
    Q_ASSERT(mOutputCells.size() > pOutput);
    if (mOutputCells[pOutput].first != nullptr) // If successor exists
    {
        mOutputCells[pOutput].first->InputReady(mOutputCells[pOutput].second, pState);
    }
}

void LogicBaseCell::InputReady(uint32_t pInput, LogicState pState)
{
    mInputStates[pInput] = pState;
    LogicFunction();
}
