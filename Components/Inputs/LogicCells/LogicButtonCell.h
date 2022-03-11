#ifndef LOGICBUTTONCELL_H
#define LOGICBUTTONCELL_H

#include "Components/LogicBaseCell.h"

#include <QTimer>

class LogicButtonCell : public LogicBaseCell
{
public:
    LogicButtonCell(void);

    void ButtonClick(void);
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    void OnShutdown(void) override;
    void OnWakeUp(void) override;

protected:
    void ButtonTimeout(void);

protected:
    LogicState mState;
    bool mStateChanged;

    QTimer mButtonTimer;
};

#endif // LOGICBUTTONCELL_H
