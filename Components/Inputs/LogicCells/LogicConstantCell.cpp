#include "LogicConstantCell.h"

LogicConstantCell::LogicConstantCell(LogicState pConstantState):
    LogicBaseCell(0, 1),
    mConstantState(pConstantState)
{}

LogicState LogicConstantCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentOutputStates[0];
}

void LogicConstantCell::OnWakeUp()
{
    mCurrentOutputStates = std::vector<LogicState>{1, mConstantState};
    mNextOutputStates = std::vector<LogicState>{1, mConstantState};
    mIsActive = true;
    mStateChanged = true;
}

void LogicConstantCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = false;
    mStateChanged = true;
}

LogicState LogicConstantCell::GetConstantState() const
{
    return mConstantState;
}
