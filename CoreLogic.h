#ifndef CORELOGIC_H
#define CORELOGIC_H

#include "View/View.h"
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

    void SetComponentType(ComponentType pComponentType);
    ComponentType GetComponentType(void);

    void EnterAddControlMode(ComponentType pComponentType);

    void Undo(void);
    void Redo(void);

    void AddCurrentTypeComponent(QPointF pPosition);

    void CopySelectedComponents(void);
    void DeleteSelectedComponents(void);

signals:
    void ControlModeChangedSignal(ControlMode pNewMode);
    void ComponentTypeChangedSignal(ComponentType pNewType);

protected:
    void ConnectToView(void);

protected:
    View &mView;

    ControlMode mControlMode = ControlMode::EDIT;

    ComponentType mComponentType = ComponentType::NONE;
    Direction mComponentDirection = components::DEFAULT_DIRECTION;
    uint8_t mComponentInputCount = components::gates::DEFAULT_INPUT_COUNT;
};

#endif // CORELOGIC_H
