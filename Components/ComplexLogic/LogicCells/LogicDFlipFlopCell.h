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
    void OnWakeUp(void) override;

protected:
    std::vector<LogicState> mOutputStates;
    std::vector<LogicState> mPrevInputStates;

    bool mStateChanged;
};

#endif // LOGICDFLIPFLOPCELL_H
