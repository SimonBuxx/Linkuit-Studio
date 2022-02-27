#include "LogicOutputCell.h"

#include <iostream>

LogicOutputCell::LogicOutputCell():
    LogicBaseCell(1, 0),
    mState(LogicState::LOW)
{}

void LogicOutputCell::LogicFunction()
{
    if (mState != mInputStates[0])
    {
        std::cout << "Output " << (mInputStates[0] == LogicState::HIGH) << std::endl;
        mState = mInputStates[0];
        emit StateChangedSignal();
    }
}

LogicState LogicOutputCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mState;
}

void LogicOutputCell::OnShutdown()
{
    mState = LogicState::LOW;
    emit StateChangedSignal();
}
