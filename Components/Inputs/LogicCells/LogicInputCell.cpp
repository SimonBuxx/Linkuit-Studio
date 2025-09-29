#include "LogicInputCell.h"

LogicInputCell::LogicInputCell():
    LogicBaseCell(0, 1)
{}

void LogicInputCell::ToggleState()
{
    mNextOutputStates[0] = ((mCurrentOutputStates[0] == LogicState::HIGH) ? LogicState::LOW : LogicState::HIGH);
    mStateChanged = true;
}

LogicState LogicInputCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentOutputStates[0];
}

void LogicInputCell::OnWakeUp()
{
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = true;
    mStateChanged = true;
}

void LogicInputCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = false;
    mStateChanged = true;
}
