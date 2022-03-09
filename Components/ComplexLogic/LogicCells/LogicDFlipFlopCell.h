#ifndef LOGICDFLIPFLOPCELL_H
#define LOGICDFLIPFLOPCELL_H

#include "Components/LogicBaseCell.h"

class LogicDFlipFlopCell : public LogicBaseCell
{
public:
    LogicDFlipFlopCell(void);

    void LogicFunction(void) override;
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;
    void OnWakeUp(void);

protected:
    std::vector<LogicState> mPreviousState;
    std::vector<LogicState> mCurrentState;

    LogicState mPreviousClockState;
    bool mStateChanged;
};

#endif // LOGICDFLIPFLOPCELL_H
