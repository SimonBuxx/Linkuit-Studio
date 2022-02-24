#include "LogicNotGateCell.h"

LogicNotGateCell::LogicNotGateCell():
    LogicBaseCell(1, 1)
{}

void LogicNotGateCell::LogicFunction()
{
    switch (mInputStates[0])
    {
        case LogicState::LOW:
        {
            NotifySuccessor(0, LogicState::HIGH);
            break;
        }
        case LogicState::HIGH:
        {
            NotifySuccessor(0, LogicState::LOW);
            break;
        }
        default:
        {
            break;
        }
    }
}

void LogicNotGateCell::Shutdown()
{
    mInputStates[0] = LogicState::LOW;
}
