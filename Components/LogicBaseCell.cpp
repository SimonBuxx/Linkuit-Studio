#include "LogicBaseCell.h"

#include <QThread>
#include <QDebug>

LogicBaseCell::LogicBaseCell(uint32_t pInputs, uint32_t pOutputs):
    mInputStates(pInputs, LogicState::LOW),
    mInputInverted(pInputs, false),
    mOutputCells(pOutputs, std::make_pair(nullptr, 0)),
    mNextUpdateTime(UpdateTime::INF)
{}

void LogicBaseCell::ConnectOutput(std::shared_ptr<LogicBaseCell> pCell, uint32_t pInput, uint32_t pOutput)
{
    mOutputCells[pOutput] = std::make_pair(pCell, pInput);
}

LogicState LogicBaseCell::GetInputState(uint32_t pInput) const
{
    Q_ASSERT(mInputStates.size() > pInput);
    return mInputStates[pInput];
}

std::vector<bool> LogicBaseCell::GetInputInversions() const
{
    return mInputInverted;
}

void LogicBaseCell::SetInputInversions(std::vector<bool> pInputInversions)
{
    mInputInverted = pInputInversions;
}

bool LogicBaseCell::IsInputInverted(uint32_t pInput) const
{
    Q_ASSERT(mInputInverted.size() > pInput);
    return mInputInverted[pInput];
}

void LogicBaseCell::NotifySuccessor(uint32_t pOutput, LogicState pState) const
{
    Q_ASSERT(mOutputCells.size() > pOutput);
    if (mOutputCells[pOutput].first != nullptr) // If successor exists
    {
        if (mOutputCells[pOutput].first->IsInputInverted(mOutputCells[pOutput].second))
        {
            mOutputCells[pOutput].first->InputReady(mOutputCells[pOutput].second, InvertState(pState));
        }
        else
        {
            mOutputCells[pOutput].first->InputReady(mOutputCells[pOutput].second, pState);
        }
    }
}

void LogicBaseCell::InputReady(uint32_t pInput, LogicState pState)
{
    if (mInputStates[pInput] != pState)
    {
        mInputStates[pInput] = pState;
        mNextUpdateTime = UpdateTime::NEXT_TICK;
    }
}

void LogicBaseCell::AdvanceUpdateTime()
{
    switch (mNextUpdateTime)
    {
        case UpdateTime::NEXT_TICK:
        {
            mNextUpdateTime = UpdateTime::NOW; // Update in next cycle
            break;
        }
        case UpdateTime::NOW:
        {
            LogicFunction(); // Update output states now
            mNextUpdateTime = UpdateTime::INF;
            break;
        }
        case UpdateTime::INF:
        {
            break; // No update scheduled
        }
    }
}
