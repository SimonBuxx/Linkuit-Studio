#ifndef LOGICANDGATECELL_H
#define LOGICANDGATECELL_H

#include "LogicBaseCell.h"

class LogicAndGateCell : public LogicBaseCell
{
public:
    LogicAndGateCell(uint32_t pInputs);

    void LogicFunction(void) override;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;

protected:
    LogicState mOutputState;
    bool mStateChanged;
};

#endif // LOGICANDGATECELL_H
