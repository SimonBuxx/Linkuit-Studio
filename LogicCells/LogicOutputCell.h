#ifndef LOGICOUTPUTCELL_H
#define LOGICOUTPUTCELL_H

#include "LogicBaseCell.h"

class LogicOutputCell : public LogicBaseCell
{
public:
    LogicOutputCell();

    void LogicFunction(void) override;

    LogicState GetState(void) const;

    void Shutdown(void) override;

protected:
    LogicState mState;
};

#endif // LOGICOUTPUTCELL_H
