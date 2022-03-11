#ifndef LOGICINPUTCELL_H
#define LOGICINPUTCELL_H

#include "Components/LogicBaseCell.h"

class LogicInputCell : public LogicBaseCell
{
public:
    LogicInputCell(void);

    void ToggleState(void);
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    void OnShutdown(void) override;
    void OnWakeUp(void) override;

protected:
    LogicState mState;
};

#endif // LOGICINPUTCELL_H
