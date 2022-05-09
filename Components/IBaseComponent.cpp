#include "IBaseComponent.h"
#include "View/View.h"
#include "HelperFunctions.h"

#include <QApplication>
#include <QGraphicsScene>

IBaseComponent::IBaseComponent(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell):
    mMoveStartPoint(pos()),
    mSimulationRunning(false),
    mLogicCell(pLogicCell)
{
    Q_UNUSED(pCoreLogic);
    setPos(0, 0);
    setCursor(Qt::PointingHandCursor);
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal, this, [&]()
    {
        setCursor(Qt::ArrowCursor);
        setFlag(ItemIsSelectable, false);
        setFlag(ItemIsMovable, false);
        setAcceptHoverEvents(false);
        mSimulationRunning = true;
    });
    QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, this, [&]()
    {
        setCursor(Qt::PointingHandCursor);
        setFlags(ItemIsSelectable | ItemIsMovable);
        setAcceptHoverEvents(true);
        mSimulationRunning = false;
    });

    QObject::connect(this, &IBaseComponent::SelectedComponentMovedSignal, pCoreLogic, &CoreLogic::OnSelectedComponentsMoved);

    QObject::connect(this, &IBaseComponent::DisplaySpecialTabSignal, pCoreLogic, &CoreLogic::OnDisplayTabRequest);

    if (mLogicCell != nullptr)
    {
        QObject::connect(pCoreLogic, &CoreLogic::SimulationAdvanceSignal, mLogicCell.get(), &LogicBaseCell::OnSimulationAdvance);
        QObject::connect(pCoreLogic, &CoreLogic::SimulationStopSignal, mLogicCell.get(), &LogicBaseCell::OnShutdown);
        QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal,mLogicCell.get(), &LogicBaseCell::OnWakeUp);
        QObject::connect(mLogicCell.get(), &LogicBaseCell::StateChangedSignal, this, &IBaseComponent::OnLogicStateChanged);
    }
}

const std::vector<LogicConnector>& IBaseComponent::GetInConnectors() const
{
    return mInConnectors;
}

const std::vector<LogicConnector>& IBaseComponent::GetOutConnectors() const
{
    return mOutConnectors;
}

uint32_t IBaseComponent::GetInConnectorCount() const
{
    return mInConnectors.size();
}

uint32_t IBaseComponent::GetOutConnectorCount() const
{
    return mOutConnectors.size();
}

void IBaseComponent::OnLogicStateChanged()
{
    update();
}

std::shared_ptr<LogicBaseCell> IBaseComponent::GetLogicCell()
{
    return mLogicCell;
}

std::optional<const LogicConnector*> IBaseComponent::InvertConnectorByPoint(QPointF pPoint)
{
    Q_ASSERT(mLogicCell);
    for (const auto& connector : mInConnectors)
    {
        if (EuclidianDistance(pos() + connector.pos + connector.pinCenterOffset, pPoint) < 6)
        {
            mLogicCell->InvertInput(connector.num);
            update();
            return &connector;
        }
    }
    for (const auto& connector : mOutConnectors)
    {
        if (EuclidianDistance(pos() + connector.pos + connector.pinCenterOffset, pPoint) < 6)
        {
            mLogicCell->InvertOutput(connector.num);
            update();
            return &connector;
        }
    }

    return std::nullopt;
}

void IBaseComponent::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsItem::mousePressEvent(pEvent);
    if (this->isSelected())
    {
        mMoveStartPoint = pos();
        setZValue(components::zvalues::FRONT);
        update();
    }
}

void IBaseComponent::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsItem::mouseReleaseEvent(pEvent);

    if (this->isSelected())
    {
        setPos(SnapToGrid(pos()));
        QPointF offset = pos() - mMoveStartPoint;

        if (offset.manhattanLength() > 0)
        {
            emit SelectedComponentMovedSignal(offset);
        }
        ResetZValue();
        update();
    }
}

QPainterPath IBaseComponent::shape() const
{
    return mShape;
}
