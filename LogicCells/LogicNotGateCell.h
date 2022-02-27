#ifndef LOGICNOTGATECELL_H
#define LOGICNOTGATECELL_H

#include "LogicBaseCell.h"

class LogicNotGateCell : public LogicBaseCell
{
public:
    LogicNotGateCell();

    void LogicFunction(void) override;
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;

protected:
    LogicState mCurrentState;
    LogicState mNextState;
    bool mStateChanged;
};

#endif // LOGICNOTGATECELL_H
