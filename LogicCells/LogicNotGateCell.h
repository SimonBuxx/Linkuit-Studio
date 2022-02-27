#ifndef LOGICNOTGATECELL_H
#define LOGICNOTGATECELL_H

#include "LogicBaseCell.h"

class LogicNotGateCell : public LogicBaseCell
{
public:
    LogicNotGateCell();

    void LogicFunction(void) override;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;

protected:
    LogicState mOutputState;
    bool mStateChanged;
};

#endif // LOGICNOTGATECELL_H
