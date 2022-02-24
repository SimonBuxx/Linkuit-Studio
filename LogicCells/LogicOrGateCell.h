#ifndef LOGICORGATECELL_H
#define LOGICORGATECELL_H

#include "LogicBaseCell.h"

class LogicOrGateCell : public LogicBaseCell
{
public:
    LogicOrGateCell(uint32_t pInputs);

    void LogicFunction(void) override;

    void Shutdown(void) override;
};

#endif // LOGICORGATECELL_H
