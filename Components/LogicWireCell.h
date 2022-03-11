#ifndef LOGICWIRECELL_H
#define LOGICWIRECELL_H

#include "LogicBaseCell.h"

class CoreLogic;

class LogicWireCell : public LogicBaseCell
{
public:
    LogicWireCell(const CoreLogic* pCoreLogic);

    void LogicFunction(void) override;

    LogicState GetOutputState(uint32_t pOutput = 0) const override;

    void AppendOutput(std::shared_ptr<LogicBaseCell> pCell, uint32_t pInput);
    void AddInputSlot(void);
    uint32_t GetInputSize(void) const;

    void InputReady(uint32_t pInput, LogicState pState) override;

public slots:
    void OnShutdown(void) override;
    void OnWakeUp(void) override;

protected:
    LogicState mState;
};

#endif // LOGICWIRECELL_H
