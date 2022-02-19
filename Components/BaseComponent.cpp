#include "BaseComponent.h"
#include "View/View.h"
#include "HelperFunctions.h"

#include <QApplication>
#include <QGraphicsScene>

BaseComponent::BaseComponent(const CoreLogic* pCoreLogic):
    mMoveStartPoint(pos())
{
    Q_UNUSED(pCoreLogic);
    setPos(0, 0);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setCursor(Qt::PointingHandCursor);

    // Connect to CoreLogic
}

void BaseComponent::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsItem::mousePressEvent(pEvent);
    mMoveStartPoint = pos();
    update();
}

void BaseComponent::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsItem::mouseMoveEvent(pEvent);
    update();
}

void BaseComponent::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsItem::mouseReleaseEvent(pEvent);

    setPos(SnapToGrid(pos()));
    QPointF offset = pos() - mMoveStartPoint;

    if (offset.manhattanLength() > 0)
    {
        if (scene() && scene()->views()[0] && static_cast<GraphicsView*>(scene()->views()[0])->GetView())
        {
            auto view = static_cast<GraphicsView*>(scene()->views()[0])->GetView();
            view->OnComponentPositionChanged(offset);
        }
    }
    update();
}
