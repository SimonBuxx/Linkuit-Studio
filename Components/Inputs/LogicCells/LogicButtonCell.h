#ifndef LOGICBUTTONCELL_H
#define LOGICBUTTONCELL_H

#include "Components/LogicBaseCell.h"

#include <QTimer>

class LogicButtonCell : public LogicBaseCell
{
public:
    LogicButtonCell(void);

    void LogicFunction(void) override;

    void ButtonClick(void);
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    void OnSimulationAdvance(void) override;
    void OnShutdown(void) override;

protected:
    LogicState mState;
    bool mStateChanged;

    QTimer mButtonTimer;
};

#endif // LOGICBUTTONCELL_H
