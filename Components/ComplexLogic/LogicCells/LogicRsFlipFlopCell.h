#ifndef LOGICRSFLIPFLOPCELL_H
#define LOGICRSFLIPFLOPCELL_H

#include "Components/LogicBaseCell.h"

class LogicRsFlipFlopCell : public LogicBaseCell
{
public:
    LogicRsFlipFlopCell(void);

    void LogicFunction(void) override;
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;
    void OnWakeUp(void);

protected:
    std::vector<LogicState> mPreviousState;
    std::vector<LogicState> mCurrentState;
    bool mStateChanged;
};

#endif // LOGICRSFLIPFLOPCELL_H
