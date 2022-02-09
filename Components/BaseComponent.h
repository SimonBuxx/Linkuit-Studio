#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class BaseComponent : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    BaseComponent(void);
    virtual BaseComponent* CloneBaseComponent() const = 0;

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    virtual void ResetZValue(void) = 0;

protected:
    uint32_t mWidth;
    uint32_t mHeight;

    QPointF mMoveStartPoint;
};

#endif // BASECOMPONENT_H
