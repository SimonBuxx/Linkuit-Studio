#include "LogicButtonCell.h"
#include "Configuration.h"

LogicButtonCell::LogicButtonCell():
    LogicBaseCell(0, 1),
    mState(LogicState::LOW),
    mStateChanged(true),
    mButtonTimer(this)
{
    mButtonTimer.setInterval(components::inputs::BUTTON_TOGGLE_INTERVAL);
    mButtonTimer.setSingleShot(true);
    QObject::connect(&mButtonTimer, &QTimer::timeout, [&](){
        if (mState != LogicState::LOW)
        {
            mState = LogicState::LOW;
            mStateChanged = true;
            emit StateChangedSignal();
        }
    });
}

void LogicButtonCell::LogicFunction()
{}

void LogicButtonCell::ButtonClick()
{
    if (mState != LogicState::HIGH)
    {
        mStateChanged = true;
        emit StateChangedSignal();
    }
    mState = LogicState::HIGH;
    mButtonTimer.start();
}

void LogicButtonCell::OnSimulationAdvance()
{
    if (mStateChanged)
    {
        NotifySuccessor(0, mState);
        mStateChanged = false;
    }
}

LogicState LogicButtonCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mState;
}

void LogicButtonCell::OnShutdown()
{
    mState = LogicState::LOW;
    mStateChanged = true;
    emit StateChangedSignal();
}
