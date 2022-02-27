#include "BaseComponent.h"
#include "View/View.h"
#include "HelperFunctions.h"

#include <QApplication>
#include <QGraphicsScene>

BaseComponent::BaseComponent(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell):
    mMoveStartPoint(pos()),
    mSimulationRunning(false),
    mLogicCell(pLogicCell)
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
        mSimulationRunning = true;
    });
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, this, [&](){
        setCursor(Qt::PointingHandCursor);
        setFlags(ItemIsSelectable | ItemIsMovable);
        setAcceptHoverEvents(true);
        mSimulationRunning = false;
    });
    QObject::connect(this, &BaseComponent::SelectedComponentMovedSignal, pCoreLogic, &CoreLogic::OnSelectedComponentsMoved);

    if (mLogicCell != nullptr)
    {
        QObject::connect(pCoreLogic, &CoreLogic::SimulationAdvanceSignal, mLogicCell.get(), &LogicBaseCell::OnSimulationAdvance);
        QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, mLogicCell.get(), &LogicBaseCell::OnShutdown);
        QObject::connect(mLogicCell.get(), &LogicBaseCell::StateChangedSignal, this, &BaseComponent::OnLogicStateChanged);
    }
}

std::vector<LogicConnector>& BaseComponent::GetInConnectors()
{
    return mInConnectors;
}

std::vector<LogicConnector>& BaseComponent::GetOutConnectors()
{
    return mOutConnectors;
}

uint32_t BaseComponent::GetInConnectorCount() const
{
    return mInConnectors.size();
}

uint32_t BaseComponent::GetOutConnectorCount() const
{
    return mOutConnectors.size();
}

void BaseComponent::OnLogicStateChanged()
{
    update();
}

std::shared_ptr<LogicBaseCell> BaseComponent::GetLogicCell()
{
    return mLogicCell;
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
