#include "LogicAndGateCell.h"

LogicAndGateCell::LogicAndGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1)
{}

void LogicAndGateCell::LogicFunction()
{
    for (const auto& input : mInputStates)
    {
        if (input != LogicState::HIGH)
        {
            NotifySuccessor(0, LogicState::LOW);
            return;
        }
    }
    NotifySuccessor(0, LogicState::HIGH);
}

void LogicAndGateCell::Shutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
}
