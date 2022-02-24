#ifndef LOGICINPUTCELL_H
#define LOGICINPUTCELL_H

#include "LogicBaseCell.h"

class LogicInputCell : public LogicBaseCell
{
public:
    LogicInputCell();

    void LogicFunction(void) override;

    void ToggleState(void);
    void SetState(LogicState pState);
    LogicState GetState(void) const;

    void Shutdown(void) override;

protected:
    LogicState mState;
};

#endif // LOGICINPUTCELL_H
