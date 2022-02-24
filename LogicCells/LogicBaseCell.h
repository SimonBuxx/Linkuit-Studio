#ifndef LOGICBASECELL_H
#define LOGICBASECELL_H

#include "HelperStructures.h"

#include <cstdint>
#include <vector>
#include <iostream>

class LogicBaseCell
{
public:
    LogicBaseCell(uint32_t pInputs, uint32_t pOutputs);
    virtual ~LogicBaseCell() = default;

    virtual void LogicFunction(void);
    void NotifySuccessor(uint32_t pOutput, LogicState pState) const;
    void InputReady(uint32_t pInput, LogicState pState);

    virtual void Shutdown(void);

    void ConnectOutput(LogicBaseCell* pCell, uint32_t pInput, uint32_t pOutput);

    void SetInput(uint32_t pInput, LogicState pState);

    uint32_t InputsSize(void) const;

protected:
    std::vector<LogicState> mInputStates;

    // Pairs of connected LogicCell and input number of that cell
    std::vector<std::pair<LogicBaseCell*, uint32_t>> mOutputCells;
};

#endif // LOGICBASECELL_H
