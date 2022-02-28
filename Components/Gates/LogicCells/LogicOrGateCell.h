#ifndef LOGICORGATECELL_H
#define LOGICORGATECELL_H

#include "Components/LogicBaseCell.h"

class LogicOrGateCell : public LogicBaseCell
{
public:
    LogicOrGateCell(uint32_t pInputs);

    void LogicFunction(void) override;
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;

protected:
    LogicState mPreviousState;
    LogicState mCurrentState;
    bool mStateChanged;
};

#endif // LOGICORGATECELL_H
