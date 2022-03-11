#ifndef LOGICOUTPUTCELL_H
#define LOGICOUTPUTCELL_H

#include "Components/LogicBaseCell.h"

class LogicOutputCell : public LogicBaseCell
{
public:
    LogicOutputCell();

    void LogicFunction(void) override;
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

    void InputReady(uint32_t pInput, LogicState pState) override;

public slots:
    void OnShutdown(void) override;

protected:
    LogicState mState;
};

#endif // LOGICOUTPUTCELL_H
