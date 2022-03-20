#include "AbstractComplexLogic.h"
#include "Configuration.h"

AbstractComplexLogic::AbstractComplexLogic(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell, uint8_t pInputCount, uint8_t pOutputCount, Direction pDirection):
    IBaseComponent(pCoreLogic, pLogicCell),
    mInputCount(pInputCount),
    mOutputCount(pOutputCount),
    mDirection(pDirection)
{
    setZValue(components::zvalues::GATE);

    if (mDirection == Direction::RIGHT || mDirection == Direction::LEFT)
    {
        mWidth = components::gates::GRID_WIDTH * canvas::GRID_SIZE;
        mHeight = std::max(mInputCount, mOutputCount) * canvas::GRID_SIZE * 2;
    }
    else
    {
        mWidth = std::max(mInputCount, mOutputCount) * canvas::GRID_SIZE * 2;
        mHeight = components::gates::GRID_WIDTH * canvas::GRID_SIZE;
    }

    SetLogicConnectors();
}

void AbstractComplexLogic::SetLogicConnectors()
{
    switch(mDirection)
    {
        case Direction::RIGHT:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE), i, QPointF(-4, 0)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE), i, QPointF(4, 0)));
            }
            break;
        }
        case Direction::DOWN:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0), i, QPointF(0, -4)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight), i, QPointF(0, 4)));
            }
            break;
        }
        case Direction::LEFT:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(mWidth, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE), i, QPointF(4, 0)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(0, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE), i, QPointF(-4, 0)));
            }
            break;
        }
        case Direction::UP:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight), i, QPointF(0, 4)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0), i, QPointF(0, -4)));
            }
            break;
        }
        default:
        {
            Q_ASSERT(false);
            break;
        }
    }
}

void AbstractComplexLogic::ResetZValue()
{
    setZValue(components::zvalues::GATE);
}

void AbstractComplexLogic::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget)
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
                DrawComponentDetailsRight(pPainter, pItem);
                break;
            }
            case Direction::DOWN:
            {
                DrawComponentDetailsDown(pPainter, pItem);
                break;
            }
            case Direction::LEFT:
            {
                DrawComponentDetailsLeft(pPainter, pItem);
                break;
            }
            case Direction::UP:
            {
                DrawComponentDetailsUp(pPainter, pItem);
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
        pPainter->setPen(components::complex_logic::FONT_COLOR);
        pPainter->setFont(components::complex_logic::FONT);
        pPainter->drawText(boundingRect(), mComponentText, Qt::AlignHCenter | Qt::AlignVCenter);
    }

    if (levelOfDetail >= components::SMALL_TEXT_MIN_LOD)
    {
        pPainter->setPen(QColor(225, 225, 225));
        pPainter->setFont(components::complex_logic::CONNECTOR_FONT);
        switch (mDirection)
        {
            case Direction::RIGHT:
            {
                for (size_t i = 0; i < mInputCount; i++)
                {
                    pPainter->drawText(QRect(2, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 6, mWidth - 4, 10), mInputLabels[i], Qt::AlignLeft | Qt::AlignVCenter);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(2, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 6, mWidth - 4, 10), mOutputLabels[i], Qt::AlignRight | Qt::AlignVCenter);
                }
                break;
            }
            case Direction::DOWN:
            {
                for (size_t i = 0; i < mInputCount; i++)
                {
                    pPainter->drawText(QRect(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 20, 0, 40, mHeight), mInputLabels[i], Qt::AlignHCenter | Qt::AlignTop);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 20, 0, 40, mHeight), mOutputLabels[i], Qt::AlignHCenter | Qt::AlignBottom);
                }
                break;
            }
            case Direction::LEFT:
            {
                for (size_t i = 0; i < mInputCount; i++)
                {
                    pPainter->drawText(QRect(2, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 6, mWidth - 4, 10), mInputLabels[i], Qt::AlignRight | Qt::AlignVCenter);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(2, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 6, mWidth - 4, 10), mOutputLabels[i], Qt::AlignLeft | Qt::AlignVCenter);
                }
                break;
            }
            case Direction::UP:
            {
                for (size_t i = 0; i < mInputCount; i++)
                {
                    pPainter->drawText(QRect(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 20, 0, 40, mHeight), mInputLabels[i], Qt::AlignHCenter | Qt::AlignBottom);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 20, 0, 40, mHeight), mOutputLabels[i], Qt::AlignHCenter | Qt::AlignTop);
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsRight(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw input connectors
    for (size_t i = 0; i < mInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(-8, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mWidth + 8, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw inversion circles
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(-9, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth + 1, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsDown(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw input connectors
    for (size_t i = 0; i < mInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, -8, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight + 8);
    }

    // Draw inversion circles
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, -9, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, mHeight + 1, 8, 8);
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsLeft(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw input connectors
    for (size_t i = 0; i < mInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mWidth + 8, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(-8, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw inversion circles
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth + 1, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(-9, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsUp(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw input connectors
    for (size_t i = 0; i < mInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight + 8);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, -8, 2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0);
    }

    // Draw inversion circles
    for (size_t i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, mHeight + 1, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(2 * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, -9, 8, 8);
        }
    }
}

void AbstractComplexLogic::SetConnectorPen(QPainter *pPainter, LogicState pState, bool pSelected)
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

void AbstractComplexLogic::SetInversionPen(QPainter *pPainter, LogicState pState, bool pSelected)
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

QRectF AbstractComplexLogic::boundingRect() const
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

QPainterPath AbstractComplexLogic::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, mWidth, mHeight);
    return path;
}
