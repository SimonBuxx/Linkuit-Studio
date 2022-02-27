#ifndef LOGICANDGATECELL_H
#define LOGICANDGATECELL_H

#include "LogicBaseCell.h"

class LogicAndGateCell : public LogicBaseCell
{
public:
    LogicAndGateCell(uint32_t pInputs);

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

#endif // LOGICANDGATECELL_H
