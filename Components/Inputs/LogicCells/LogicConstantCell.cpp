#include "LogicConstantCell.h"

LogicConstantCell::LogicConstantCell(LogicState pConstantState):
    LogicBaseCell(0, 1),
    mState(LogicState::LOW),
    mConstantState(pConstantState),
    mStateChanged(true)
{}

LogicState LogicConstantCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mState;
}

void LogicConstantCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mState);

        emit StateChangedSignal();
    }
}

void LogicConstantCell::OnWakeUp()
{
    mState = mConstantState;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicConstantCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mState = LogicState::LOW;
    mIsActive = false;
    emit StateChangedSignal();
}

LogicState LogicConstantCell::GetConstantState() const
{
    return mConstantState;
}
