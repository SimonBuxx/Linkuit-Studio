#include "LogicXorGateCell.h"

LogicXorGateCell::LogicXorGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1)
{}

void LogicXorGateCell::LogicFunction()
{
    bool oneHigh = false;
    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            if (oneHigh)
            {
                NotifySuccessor(0, LogicState::LOW);
                return;
            }
            oneHigh = true;
        }
    }

    NotifySuccessor(0, oneHigh ? LogicState::HIGH : LogicState::LOW);
}

void LogicXorGateCell::Shutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
}
