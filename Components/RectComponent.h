#ifndef RECTCOMPONENT_H
#define RECTCOMPONENT_H

#include "BaseComponent.h"
#include "Configuration.h"

/// \brief Base class for all rectangular components
class RectComponent : public BaseComponent
{
    Q_OBJECT
public:
    RectComponent(void);
    virtual BaseComponent* CloneBaseComponent() const override = 0;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

signals:
    void ItemMovedSignal(QPointF pNewPosition);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    virtual void PaintSpecifics(QPainter *pPainter, const double pLevelOfDetail) = 0;

protected:
    uint32_t mWidth;
    uint32_t mHeight;
};

#endif // RECTCOMPONENT_H
