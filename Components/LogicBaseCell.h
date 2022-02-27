#ifndef LOGICBASECELL_H
#define LOGICBASECELL_H

#include "HelperStructures.h"

#include <QObject>

class LogicBaseCell: public QObject
{
    Q_OBJECT
public:
    LogicBaseCell(uint32_t pInputs, uint32_t pOutputs);
    virtual ~LogicBaseCell() = default;

    virtual void LogicFunction(void);
    void NotifySuccessor(uint32_t pOutput, LogicState pState) const;
    void InputReady(uint32_t pInput, LogicState pState);

    void ConnectOutput(std::shared_ptr<LogicBaseCell> pCell, uint32_t pInput, uint32_t pOutput);

    LogicState GetInputState(uint32_t pInput) const;
    virtual LogicState GetOutputState(uint32_t pOutput = 0) const = 0;

public slots:
    virtual void OnSimulationAdvance(void);
    virtual void OnShutdown(void);

signals:
    void StateChangedSignal(void);

protected:
    std::vector<LogicState> mInputStates;

    // Pairs of connected LogicCell and input number of that cell
    std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>> mOutputCells;
};

#endif // LOGICBASECELL_H
