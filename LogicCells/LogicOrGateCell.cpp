#include "LogicOrGateCell.h"

LogicOrGateCell::LogicOrGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1)
{}

void LogicOrGateCell::LogicFunction()
{
    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            NotifySuccessor(0, LogicState::HIGH);
            return;
        }
    }
    NotifySuccessor(0, LogicState::LOW);
}

void LogicOrGateCell::Shutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
}
