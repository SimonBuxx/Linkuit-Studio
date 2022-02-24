#ifndef LOGICXORGATECELL_H
#define LOGICXORGATECELL_H

#include "LogicBaseCell.h"

class LogicXorGateCell : public LogicBaseCell
{
public:
    LogicXorGateCell(uint32_t pInputs);

    void LogicFunction(void) override;

    void Shutdown(void) override;
};

#endif // LOGICXORGATECELL_H
