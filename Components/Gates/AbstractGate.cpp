#include "AbstractGate.h"
#include "Configuration.h"

AbstractGate::AbstractGate(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell, uint8_t pInputCount, Direction pDirection):
    IBaseComponent(pCoreLogic, pLogicCell),
    mInputCount(pInputCount),
    mDirection(pDirection)
{
    Q_ASSERT(mInputCount >= 1);

    setZValue(components::zvalues::GATE);

    if (mDirection == Direction::RIGHT || mDirection == Direction::LEFT)
    {
        mWidth = components::gates::GRID_WIDTH * canvas::GRID_SIZE;
        mHeight = mInputCount * canvas::GRID_SIZE * 2;
    }
    else
    {
        mWidth = mInputCount * canvas::GRID_SIZE * 2;
        mHeight = components::gates::GRID_WIDTH * canvas::GRID_SIZE;
    }

    SetLogicConnectors();
}

void AbstractGate::SetLogicConnectors()
{
    switch(mDirection)
    {
        case Direction::RIGHT:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE), i, QPointF(-4, 0)));
            }
            mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth, mHeight / 2), 0, QPointF(4, 0)));
            break;
        }
        case Direction::DOWN:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0), i, QPointF(0, -4)));
            }
            mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth / 2, mHeight), 0, QPointF(0, 4)));
            break;
        }
        case Direction::LEFT:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(mWidth, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE), i, QPointF(4, 0)));
            }
            mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(0, mHeight / 2), 0, QPointF(-4, 0)));
            break;
        }
        case Direction::UP:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight), i, QPointF(0, 4)));
            }
            mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth / 2, 0), 0, QPointF(0, -4)));
            break;
        }
        default:
        {
            Q_ASSERT(false);
            break;
        }
    }
}

void AbstractGate::ResetZValue()
{
    setZValue(components::zvalues::GATE);
}

void AbstractGate::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget)
{
    Q_UNUSED(pWidget);
    const double levelOfDetail = pItem->levelOfDetailFromTransform(pPainter->worldTransform());

    // Draw connectors and inversion circles
    if (levelOfDetail >= components::COMPONENT_DETAILS_MIN_LOD)
    {
        switch(mDirection)
        {
            case Direction::RIGHT:
            {
                DrawGateDetailsRight(pPainter, pItem);
                break;
            }
            case Direction::DOWN:
            {
                DrawGateDetailsDown(pPainter, pItem);
                break;
            }
            case Direction::LEFT:
            {
                DrawGateDetailsLeft(pPainter, pItem);
                break;
            }
            case Direction::UP:
            {
                DrawGateDetailsUp(pPainter, pItem);
                break;
            }
            default:
            {
                Q_ASSERT(false);
                break;
            }
        }
    }

    // Draw gate body
    {
        QPen pen(pItem->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR,
                 components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::FILL_COLOR));
        pPainter->drawRect(0, 0, mWidth, mHeight);
    }

    // Draw description text
    if (levelOfDetail >= components::DESCRIPTION_TEXT_MIN_LOD)
    {
        pPainter->setPen(components::gates::FONT_COLOR);
        pPainter->setFont(components::gates::FONT);
        pPainter->drawText(boundingRect(), mComponentText, Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

void AbstractGate::DrawGateDetailsRight(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw input connectors
    for (size_t i = 0; i < mInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(-8, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw output connector
    SetConnectorPen(pPainter, mLogicCell->GetOutputState(), pItem->state & QStyle::State_Selected);
    pPainter->drawLine(mWidth, mHeight / 2, mWidth + 8, mHeight / 2);

    // Draw inversion circles
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(-9, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }

    if (mLogicCell->IsOutputInverted(0))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(), pItem->state & QStyle::State_Selected);
        pPainter->drawEllipse(mWidth + 1, mHeight / 2 - 4, 8, 8);
    }
}

void AbstractGate::DrawGateDetailsDown(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw input connectors
    for (size_t i = 0; i < mInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, -8, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0);
    }

    // Draw output connector
    SetConnectorPen(pPainter, mLogicCell->GetOutputState(), pItem->state & QStyle::State_Selected);
    pPainter->drawLine(mWidth / 2, mHeight, mWidth / 2, mHeight + 8);

    // Draw inversion circles
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, -9, 8, 8);
        }
    }

    if (mLogicCell->IsOutputInverted(0))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(), pItem->state & QStyle::State_Selected);
        pPainter->drawEllipse(mWidth / 2 - 4, mHeight + 1, 8, 8);
    }
}

void AbstractGate::DrawGateDetailsLeft(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw input connectors
    for (size_t i = 0; i < mInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mWidth + 8, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw output connector
    SetConnectorPen(pPainter, mLogicCell->GetOutputState(), pItem->state & QStyle::State_Selected);
    pPainter->drawLine(-8, mHeight / 2, 0, mHeight / 2);

    // Draw inversion circles
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth + 1, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }

    if (mLogicCell->IsOutputInverted(0))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(), pItem->state & QStyle::State_Selected);
        pPainter->drawEllipse(-9, mHeight / 2 - 4, 8, 8);
    }
}

void AbstractGate::DrawGateDetailsUp(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw input connectors
    for (size_t i = 0; i < mInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight + 8);
    }

    // Draw output connector
    SetConnectorPen(pPainter, mLogicCell->GetOutputState(), pItem->state & QStyle::State_Selected);
    pPainter->drawLine(mWidth / 2, -8, mWidth / 2, 0);

    // Draw inversion circles
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, mHeight + 1, 8, 8);
        }
    }

    if (mLogicCell->IsOutputInverted(0))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(), pItem->state & QStyle::State_Selected);
        pPainter->drawEllipse(mWidth / 2 - 4, -9, 8, 8);
    }
}

void AbstractGate::SetConnectorPen(QPainter *pPainter, LogicState pState, bool pSelected)
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

void AbstractGate::SetInversionPen(QPainter *pPainter, LogicState pState, bool pSelected)
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

QRectF AbstractGate::boundingRect() const
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

QPainterPath AbstractGate::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, mWidth, mHeight);
    return path;
}
