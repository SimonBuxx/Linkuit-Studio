#ifndef LOGICXORGATECELL_H
#define LOGICXORGATECELL_H

#include "Components/LogicBaseCell.h"

class LogicXorGateCell : public LogicBaseCell
{
public:
    LogicXorGateCell(uint32_t pInputs);

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

#endif // LOGICXORGATECELL_H
