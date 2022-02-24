#include "LogicOutputCell.h"

LogicOutputCell::LogicOutputCell():
    LogicBaseCell(1, 0),
    mState(LogicState::LOW)
{}

void LogicOutputCell::LogicFunction()
{
    std::cout << "Output " << (mInputStates[0] == LogicState::HIGH) << "\n";
    mState = mInputStates[0];
}

LogicState LogicOutputCell::GetState() const
{
    return mState;
}

void LogicOutputCell::Shutdown()
{
    mState = LogicState::LOW;
}
