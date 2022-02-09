#ifndef LOGICOUTPUT_H
#define LOGICOUTPUT_H

#include "BaseComponent.h"
#include "HelperStructures.h"

class LogicOutput : public BaseComponent
{
    Q_OBJECT
public:
    LogicOutput(void);
    LogicOutput(const LogicOutput& pObj);
    virtual BaseComponent* CloneBaseComponent() const override;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void ResetZValue(void) override;

protected:
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    LogicState mState;
};

#endif // LOGICOUTPUT_H
