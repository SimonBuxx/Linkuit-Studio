#ifndef LOGICORGATECELL_H
#define LOGICORGATECELL_H

#include "LogicBaseCell.h"

class LogicOrGateCell : public LogicBaseCell
{
public:
    LogicOrGateCell(uint32_t pInputs);

    void LogicFunction(void) override;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;

protected:
    LogicState mOutputState;
    bool mStateChanged;
};

#endif // LOGICORGATECELL_H
