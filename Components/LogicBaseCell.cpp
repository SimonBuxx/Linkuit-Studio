#include "LogicBaseCell.h"

#include <QThread>
#include <QDebug>

LogicBaseCell::LogicBaseCell(uint32_t pInputs, uint32_t pOutputs):
    mInputStates(pInputs, LogicState::LOW),
    mInputConnected(pInputs, false),
    mInputInverted(pInputs, false),
    mOutputInverted(pOutputs, false),
    mOutputCells(pOutputs, std::make_pair(nullptr, 0)),
    mNextUpdateTime(UpdateTime::INF),
    mIsActive(false)
{}

void LogicBaseCell::ConnectOutput(const std::shared_ptr<LogicBaseCell>& pLogicCell, uint32_t pInput, uint32_t pOutput)
{
    mOutputCells[pOutput] = std::make_pair(pLogicCell, pInput);
}

void LogicBaseCell::SetConnected(uint32_t pInput)
{
    mInputConnected[pInput] = true;
}

bool LogicBaseCell::IsInputConnected(uint32_t pInput)
{
    return mInputConnected[pInput];
}

bool LogicBaseCell::IsOutputConnected(uint32_t pOutput)
{
    return (nullptr != mOutputCells[pOutput].first);
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

void LogicBaseCell::SetInputInversions(const std::vector<bool>& pInputInversions)
{
    mInputInverted = pInputInversions;
}

bool LogicBaseCell::IsInputInverted(uint32_t pInput) const
{
    Q_ASSERT(mInputInverted.size() > pInput);
    return mInputInverted[pInput];
}

void LogicBaseCell::InvertInput(uint32_t pInput)
{
    Q_ASSERT(pInput < mInputInverted.size());
    mInputInverted[pInput] = !mInputInverted[pInput];
}

std::vector<bool> LogicBaseCell::GetOutputInversions() const
{
    return mOutputInverted;
}

void LogicBaseCell::SetOutputInversions(const std::vector<bool>& pOutputInversions)
{
    mOutputInverted = pOutputInversions;
}

bool LogicBaseCell::IsOutputInverted(uint32_t pOutput) const
{
    Q_ASSERT(mOutputInverted.size() > pOutput);
    return mOutputInverted[pOutput];
}

void LogicBaseCell::InvertOutput(uint32_t pOutput)
{
    Q_ASSERT(pOutput < mOutputInverted.size());
    mOutputInverted[pOutput] = !mOutputInverted[pOutput];
}

bool LogicBaseCell::IsActive() const
{
    return mIsActive;
}

void LogicBaseCell::NotifySuccessor(uint32_t pOutput, LogicState pState) const
{
    Q_ASSERT(mOutputCells.size() > pOutput);
    if (mOutputCells[pOutput].first != nullptr) // If successor exists
    {
        if (mOutputCells[pOutput].first->IsInputInverted(mOutputCells[pOutput].second))
        {
            const auto&& forwardedState = ApplyInversion(InvertState(pState), pOutput);
            mOutputCells[pOutput].first->InputReady(mOutputCells[pOutput].second, forwardedState);
        }
        else
        {
            const auto&& forwardedState = ApplyInversion(pState, pOutput);
            mOutputCells[pOutput].first->InputReady(mOutputCells[pOutput].second, forwardedState);
        }
    }
}

LogicState LogicBaseCell::ApplyInversion(LogicState pState, uint32_t pOutput) const
{
    Q_ASSERT(mOutputInverted.size() > pOutput);
    if (mOutputInverted[pOutput])
    {
        return InvertState(pState);
    }
    else
    {
        return pState;
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

bool LogicBaseCell::AssureState(LogicState &pSubject, const LogicState &pTargetState)
{
    if (pSubject != pTargetState)
    {
        pSubject = pTargetState;
        return true;
    }

    return false;
}

bool LogicBaseCell::AssureStateIf(bool pCondition, LogicState &pSubject, const LogicState &pTargetState)
{
    if (pCondition && pSubject != pTargetState)
    {
        pSubject = pTargetState;
        return true;
    }

    return false;
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
