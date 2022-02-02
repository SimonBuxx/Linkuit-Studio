#ifndef RECTCOMPONENT_H
#define RECTCOMPONENT_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "Configuration.h"

/// \brief Base class for all rectangular components
class RectComponent : public QGraphicsItem
{
public:
    RectComponent(void);

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

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
