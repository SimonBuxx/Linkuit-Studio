#ifndef LOGICINPUT_H
#define LOGICINPUT_H

#include "BaseComponent.h"
#include "HelperStructures.h"
#include "LogicCells/LogicInputCell.h"

class LogicInput : public BaseComponent
{
    Q_OBJECT
public:
    LogicInput(const CoreLogic* pCoreLogic);
    LogicInput(const LogicInput& pObj, const CoreLogic* pCoreLogic);

    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void ResetZValue(void) override;

protected:
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;
};

#endif // LOGICINPUT_H
