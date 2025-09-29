#include "IBaseComponent.h"
#include "CoreLogic.h"
#include "HelperFunctions.h"

#include <QApplication>
#include <QGraphicsScene>

IBaseComponent::IBaseComponent(const CoreLogic* pCoreLogic, const std::shared_ptr<LogicBaseCell>& pLogicCell):
    mMoveStartPoint(pos()),
    mSimulationRunning(false),
    mLogicCell(pLogicCell)
{
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

    QObject::connect(pCoreLogic, &CoreLogic::ControlModeChangedSignal, this, [&](ControlMode pNewMode)
    {
        if (pNewMode == ControlMode::EDIT || (dynamic_cast<LogicWire*>(this) == nullptr && dynamic_cast<ConPoint*>(this) == nullptr && pNewMode == ControlMode::ADD)) // Do not display hand cursor in ADD mode for wires or ConPoints
        {
            setCursor(Qt::PointingHandCursor);
        }
        else
        {
            setCursor(Qt::ArrowCursor);
        }
    });

    QObject::connect(this, &IBaseComponent::SelectedComponentMovedSignal, pCoreLogic, &CoreLogic::OnSelectedComponentsMovedOrPasted);

    if (mLogicCell != nullptr)
    {
        QObject::connect(pCoreLogic, &CoreLogic::CalculateNextStateSignal, mLogicCell.get(), &LogicBaseCell::OnCalculateNextState);
        QObject::connect(pCoreLogic, &CoreLogic::CommitStateSignal, mLogicCell.get(), &LogicBaseCell::OnCommitState);
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

IBaseComponent::~IBaseComponent(void)
{

}
