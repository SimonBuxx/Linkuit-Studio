#ifndef CORELOGIC_H
#define CORELOGIC_H

#include "View.h"
#include "Configuration.h"

#include <QGraphicsItem>

class View;

class CoreLogic : public QObject
{
    Q_OBJECT
public:
    CoreLogic(View &pView);

    QGraphicsItem* GetItem(void);

    void EnterControlMode(ControlMode pMode);
    ControlMode GetControlMode(void);

    bool IsInAddMode(void);

signals:
    void ControlModeChangedSignal(ControlMode pNewMode);

protected:
    void ConnectToView(void);

protected:
    View &mView;

    ControlMode mControlMode = ControlMode::EDIT;

    Component mAddComponent = Component::NONE;
    Direction mAddDirection = components::DEFAULT_DIRECTION;
    uint8_t mAddInputCount = components::gates::DEFAULT_INPUT_COUNT;
};

#endif // CORELOGIC_H
