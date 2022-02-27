#ifndef LOGICWIRECELL_H
#define LOGICWIRECELL_H

#include "LogicBaseCell.h"

class CoreLogic;

class LogicWireCell : public LogicBaseCell
{
public:
    LogicWireCell(const CoreLogic* pCoreLogic);

    void LogicFunction(void) override;

    LogicState GetState(void) const;

    void AppendOutput(std::shared_ptr<LogicBaseCell> pCell, uint32_t pInput);
    void AddInputSlot(void);
    uint32_t GetInputSize(void) const;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;

protected:
    LogicState mState;
    bool mStateChanged;
};

#endif // LOGICWIRECELL_H
