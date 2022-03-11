#include "LogicOutputCell.h"

LogicOutputCell::LogicOutputCell():
    LogicBaseCell(1, 0),
    mState(LogicState::LOW)
{}

void LogicOutputCell::LogicFunction()
{
    if (mState != mInputStates[0])
    {
        mState = mInputStates[0];
        emit StateChangedSignal();
    }
}

LogicState LogicOutputCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mState;
}

void LogicOutputCell::InputReady(uint32_t pInput, LogicState pState)
{
    mInputStates[pInput] = pState;
    LogicFunction();
}

void LogicOutputCell::OnShutdown()
{
    mState = LogicState::LOW;
    emit StateChangedSignal();
}
