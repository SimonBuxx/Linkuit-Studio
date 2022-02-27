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

LogicState LogicOutputCell::GetState() const
{
    return mState;
}

void LogicOutputCell::OnShutdown()
{
    mState = LogicState::LOW;
    emit StateChangedSignal();
}
