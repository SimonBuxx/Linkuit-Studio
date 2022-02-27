#ifndef LOGICXORGATECELL_H
#define LOGICXORGATECELL_H

#include "LogicBaseCell.h"

class LogicXorGateCell : public LogicBaseCell
{
public:
    LogicXorGateCell(uint32_t pInputs);

    void LogicFunction(void) override;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;

protected:
    LogicState mOutputState;
    bool mStateChanged;
};

#endif // LOGICXORGATECELL_H
