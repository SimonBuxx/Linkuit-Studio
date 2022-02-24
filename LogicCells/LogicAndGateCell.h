#ifndef LOGICANDGATECELL_H
#define LOGICANDGATECELL_H

#include "LogicBaseCell.h"

class LogicAndGateCell : public LogicBaseCell
{
public:
    LogicAndGateCell(uint32_t pInputs);

    void LogicFunction(void) override;

    void Shutdown(void) override;
};

#endif // LOGICANDGATECELL_H
