#include "LogicClock.h"
#include "CoreLogic.h"
#include "Configuration.h"

LogicClock::LogicClock(const CoreLogic* pCoreLogic, Direction pDirection):
    IBaseComponent(pCoreLogic, std::make_shared<LogicClockCell>()),
    mDirection(pDirection)
{
    setZValue(components::zvalues::INPUT);

    mWidth = canvas::GRID_SIZE * 2;
    mHeight = canvas::GRID_SIZE * 2;

    mShape.addRect(0, 0, mWidth, mHeight);

    // Construct square wave symbol
    constexpr uint8_t amplitude = 10;

    mSquareWave.moveTo(8, mHeight / 2 + amplitude);
    mSquareWave.lineTo(14, mHeight / 2 + amplitude);
    mSquareWave.lineTo(14, mHeight / 2 - amplitude);
    mSquareWave.lineTo(20, mHeight / 2 - amplitude);
    mSquareWave.lineTo(20, mHeight / 2 + amplitude);
    mSquareWave.lineTo(26, mHeight / 2 + amplitude);
    mSquareWave.lineTo(26, mHeight / 2 - amplitude);
    mSquareWave.lineTo(32, mHeight / 2 - amplitude);

    SetLogicConnectors();

    QObject::connect(this, &LogicClock::DisplayClockConfigurationSignal, pCoreLogic, &CoreLogic::OnDisplayClockConfigurationRequest);
}

LogicClock::LogicClock(const LogicClock& pObj, const CoreLogic* pCoreLogic):
    LogicClock(pCoreLogic, pObj.mDirection)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
};

void LogicClock::SetLogicConnectors()
{
    switch (mDirection)
    {
        case Direction::RIGHT:
        {
            mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth, mHeight / 2), 0, QPointF(4, 0))); // Clock output
            break;
        }
        case Direction::DOWN:
        {
            mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth / 2, mHeight), 0, QPointF(0, 4))); // Clock output
            break;
        }
        case Direction::LEFT:
        {
            mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(0, mHeight / 2), 0, QPointF(-4, 0))); // Clock output
            break;
        }
        case Direction::UP:
        {
            mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth / 2, 0), 0, QPointF(0, -4))); // Clock output
            break;
        }
        default:
        {
            throw std::logic_error("Direction invalid");
        }
    }
}

IBaseComponent* LogicClock::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new LogicClock(*this, pCoreLogic);
}

void LogicClock::ResetZValue()
{
    setZValue(components::zvalues::INPUT);
}

void LogicClock::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    // Draw connectors and inversion circles
    if (levelOfDetail >= components::COMPONENT_DETAILS_MIN_LOD)
    {
        switch(mDirection)
        {
            case Direction::RIGHT:
            {
                DrawClockDetailsRight(pPainter, pOption);
                break;
            }
            case Direction::DOWN:
            {
                DrawClockDetailsDown(pPainter, pOption);
                break;
            }
            case Direction::LEFT:
            {
                DrawClockDetailsLeft(pPainter, pOption);
                break;
            }
            case Direction::UP:
            {
                DrawClockDetailsUp(pPainter, pOption);
                break;
            }
            default:
            {
                throw std::logic_error("Direction invalid");
            }
        }
    }

    // Draw clock body
    {
        QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR,
                 components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::FILL_COLOR));
        pPainter->drawRect(0, 0, mWidth, mHeight);
    }

    // Draw square wave icon
    if (levelOfDetail >= components::DESCRIPTION_TEXT_MIN_LOD)
    {
        pPainter->setPen(QPen(components::gates::FONT_COLOR, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        pPainter->drawPath(mSquareWave);
    }
}

void LogicClock::DrawClockDetailsRight(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption)
{
    // Draw clock output connector
    SetConnectorPen(pPainter, mLogicCell->GetOutputState(), pOption->state & QStyle::State_Selected);
    pPainter->drawLine(mWidth, mHeight / 2, mWidth + 8, mHeight / 2);

    // Draw inversion circle
    if (mLogicCell->IsOutputInverted(0))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(), pOption->state & QStyle::State_Selected);
        pPainter->drawEllipse(mWidth + 1, mHeight / 2 - 4, 8, 8);
    }
}

void LogicClock::DrawClockDetailsDown(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption)
{
    // Draw clock output connector
    SetConnectorPen(pPainter, mLogicCell->GetOutputState(), pOption->state & QStyle::State_Selected);
    pPainter->drawLine(mWidth / 2, mHeight, mWidth / 2, mHeight + 8);

    // Draw inversion circle
    if (mLogicCell->IsOutputInverted(0))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(), pOption->state & QStyle::State_Selected);
        pPainter->drawEllipse(mWidth / 2 - 4, mHeight + 1, 8, 8);
    }
}

void LogicClock::DrawClockDetailsLeft(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption)
{
    // Draw clock output connector
    SetConnectorPen(pPainter, mLogicCell->GetOutputState(), pOption->state & QStyle::State_Selected);
    pPainter->drawLine(-8, mHeight / 2, 0, mHeight / 2);

    // Draw inversion circle
    if (mLogicCell->IsOutputInverted(0))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(), pOption->state & QStyle::State_Selected);
        pPainter->drawEllipse(-9, mHeight / 2 - 4, 8, 8);
    }
}

void LogicClock::DrawClockDetailsUp(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption)
{
    // Draw clock output connector
    SetConnectorPen(pPainter, mLogicCell->GetOutputState(), pOption->state & QStyle::State_Selected);
    pPainter->drawLine(mWidth / 2, -8, mWidth / 2, 0);

    // Draw inversion circle
    if (mLogicCell->IsOutputInverted(0))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(), pOption->state & QStyle::State_Selected);
        pPainter->drawEllipse(mWidth / 2 - 4, -9, 8, 8);
    }
}

void LogicClock::SetConnectorPen(QPainter *pPainter, LogicState pState, bool pSelected)
{
    if (pState == LogicState::HIGH)
    {
        pPainter->setPen(QPen(components::wires::WIRE_HIGH_COLOR, components::wires::WIRE_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else
    {
        pPainter->setPen(QPen(pSelected ? components::SELECTED_BORDER_COLOR : components::wires::WIRE_LOW_COLOR,
                              components::wires::WIRE_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
}

void LogicClock::SetInversionPen(QPainter *pPainter, LogicState pState, bool pSelected)
{
    if (pState == LogicState::HIGH)
    {
        pPainter->setPen(QPen(components::wires::WIRE_HIGH_COLOR, components::wires::WIRE_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        pPainter->setBrush(components::wires::WIRE_HIGH_COLOR);
    }
    else
    {
        pPainter->setPen(QPen(pSelected ? components::SELECTED_BORDER_COLOR : components::wires::WIRE_LOW_COLOR,
                              components::wires::WIRE_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        pPainter->setBrush(pSelected ? components::SELECTED_BORDER_COLOR : components::wires::WIRE_LOW_COLOR);
    }
}

#warning use reimplemented itemChange() instead with QGraphicsItem::ItemSelectedChange
void LogicClock::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    IBaseComponent::mousePressEvent(pEvent);
    if (this->isSelected() && this->scene()->selectedItems().size() == 1)
    {
        emit DisplayClockConfigurationSignal(std::static_pointer_cast<LogicClockCell>(mLogicCell)->GetClockMode(),
                                             std::static_pointer_cast<LogicClockCell>(mLogicCell)->GetToggleTicks(),
                                             std::static_pointer_cast<LogicClockCell>(mLogicCell)->GetPulseTicks());
    }
}

QRectF LogicClock::boundingRect() const
{
    if (mDirection == Direction::RIGHT || mDirection == Direction::LEFT)
    {
        return QRectF(-13, -3, mWidth + 26, mHeight + 6);
    }
    else
    {
        return QRectF(-3, -13, mWidth + 6, mHeight + 26);
    }
}
