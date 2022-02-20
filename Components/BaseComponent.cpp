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
    setCursor(Qt::PointingHandCursor);
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal, this, [&](){
        setCursor(Qt::ArrowCursor);
        setFlag(ItemIsSelectable, false);
        setFlag(ItemIsMovable, false);
        setAcceptHoverEvents(false);
    });
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, this, [&](){
        setCursor(Qt::PointingHandCursor);
        setFlags(ItemIsSelectable | ItemIsMovable);
        setAcceptHoverEvents(true);
    });
    QObject::connect(this, &BaseComponent::SelectedComponentMovedSignal, pCoreLogic, &CoreLogic::OnSelectedComponentsMoved);
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
        emit SelectedComponentMovedSignal(offset);
    }
    update();
}
