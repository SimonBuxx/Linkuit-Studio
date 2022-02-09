#ifndef LOGICINPUT_H
#define LOGICINPUT_H

#include "BaseComponent.h"
#include "HelperStructures.h"

class LogicInput : public BaseComponent
{
    Q_OBJECT
public:
    LogicInput(void);
    LogicInput(const LogicInput& pObj);
    virtual BaseComponent* CloneBaseComponent() const override;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void ResetZValue(void) override;

protected:
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    LogicState mState;
};

#endif // LOGICINPUT_H
