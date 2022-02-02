#include "RectComponent.h"
#include "HelperFunctions.h"

#include <QApplication>
#include <QGraphicsScene>

RectComponent::RectComponent()
{
    setPos(0, 0);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}

void RectComponent::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    QPainterPath path;
    path.addRect(0, 0, mWidth, mHeight);


    if (pOption->state & QStyle::State_Selected)
    {
        QPen pen(components::SELECTED_BORDER_COLOR, components::BORDER_WIDTH);
        pPainter->setPen(pen);
    }
    else if (pOption->state & QStyle::State_MouseOver)
    {
        QPen pen(components::HOVER_BORDER_COLOR, components::BORDER_WIDTH);
        pPainter->setPen(pen);
    }
    else
    {
        QPen pen(components::BORDER_COLOR, components::BORDER_WIDTH);
        pPainter->setPen(pen);
    }

    pPainter->fillPath(path, components::FILL_COLOR);
    pPainter->drawPath(path);

    PaintSpecifics(pPainter, levelOfDetail);
}

QRectF RectComponent::boundingRect() const
{
    return QRectF(0, 0, mWidth, mHeight);
}

QPainterPath RectComponent::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, mWidth, mHeight);
    return path;
}

QVariant RectComponent::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == QGraphicsItem::ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        if(QApplication::mouseButtons() == Qt::LeftButton && qobject_cast<QGraphicsScene*> (scene()))
        {
            return SnapToGrid(newPos);
        }
        else
        {
            return newPos;
        }
    }
    else
    {
        return QGraphicsItem::itemChange(change, value);
    }
}


void RectComponent::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsItem::mousePressEvent(pEvent);
    update();
}

void RectComponent::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsItem::mouseMoveEvent(pEvent);
    update();
}

void RectComponent::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsItem::mouseReleaseEvent(pEvent);
    update();
}
