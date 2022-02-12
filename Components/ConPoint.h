#ifndef CONPOINT_H
#define CONPOINT_H

#include "BaseComponent.h"
#include "HelperStructures.h"

class ConPoint : public BaseComponent
{
public:
    ConPoint(void);
    ConPoint(const ConPoint& pObj);

    virtual BaseComponent* CloneBaseComponent() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void ResetZValue(void) override;

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    LogicState mState;

    bool mWasMoved;
    bool mWasSelectedBefore;
};

#endif // CONPOINT_H
