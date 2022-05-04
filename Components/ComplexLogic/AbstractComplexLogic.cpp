#include "AbstractComplexLogic.h"
#include "Configuration.h"

AbstractComplexLogic::AbstractComplexLogic(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell, uint8_t pInputCount, uint8_t pOutputCount,
                                           Direction pDirection, uint8_t pTopInputCount):
    IBaseComponent(pCoreLogic, pLogicCell),
    mInputCount(pInputCount),
    mOutputCount(pOutputCount),
    mDirection(pDirection),
    mInputsSpacing(1),
    mOutputsSpacing(1),
    mTopInputCount(pTopInputCount),
    mSmallerDescription(false)
{
    setZValue(components::zvalues::GATE);

    if (mInputCount - mTopInputCount == 2)
    {
        mInputsSpacing = 2;
    }

    if (mOutputCount == 2)
    {
        mOutputsSpacing = 2;
    }

    if (mDirection == Direction::RIGHT || mDirection == Direction::LEFT)
    {
        mWidth = std::max(components::gates::GRID_WIDTH * canvas::GRID_SIZE, (components::gates::GRID_WIDTH - 2) * canvas::GRID_SIZE + mTopInputCount * canvas::GRID_SIZE);
        mHeight = std::max((mInputCount - mTopInputCount + (mInputsSpacing == 2 ? 0 : 1)) * mInputsSpacing,
                           (mOutputCount + (mOutputsSpacing == 2 ? 0 : 1)) * mOutputsSpacing) * canvas::GRID_SIZE;
    }
    else
    {
        mWidth = std::max((mInputCount - mTopInputCount + (mInputsSpacing == 2 ? 0 : 1)) * mInputsSpacing,
                          (mOutputCount + (mOutputsSpacing == 2 ? 0 : 1)) * mOutputsSpacing) * canvas::GRID_SIZE;
        mHeight = std::max(components::gates::GRID_WIDTH * canvas::GRID_SIZE, (components::gates::GRID_WIDTH - 2) * canvas::GRID_SIZE + mTopInputCount * canvas::GRID_SIZE);
    }

    mShape.addRect(0, 0, mWidth, mHeight);

    SetLogicConnectors();
}

void AbstractComplexLogic::SetLogicConnectors()
{
    switch(mDirection)
    {
        case Direction::RIGHT:
        {
            for (uint8_t i = 0; i < mTopInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(canvas::GRID_SIZE * (mTopInputCount - i - 1) + canvas::GRID_SIZE, 0), i, QPointF(0, -4)));
            }
            for (uint8_t i = 0; i < mInputCount - mTopInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE), i + mTopInputCount, QPointF(-4, 0)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth, mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE), i, QPointF(4, 0)));
            }
            break;
        }
        case Direction::DOWN:
        {
            for (uint8_t i = 0; i < mTopInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(mWidth, canvas::GRID_SIZE * (mTopInputCount - i - 1) + canvas::GRID_SIZE), i, QPointF(4, 0)));
            }
            for (uint8_t i = 0; i < mInputCount - mTopInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(mWidth - (mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE), 0), i + mTopInputCount, QPointF(0, -4)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth - (mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE), mHeight), i, QPointF(0, 4)));
            }
            break;
        }
        case Direction::LEFT:
        {
            for (uint8_t i = 0; i < mTopInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(mWidth - (canvas::GRID_SIZE * (mTopInputCount - i - 1) + canvas::GRID_SIZE), mHeight), i, QPointF(0, 4)));
            }
            for (uint8_t i = 0; i < mInputCount - mTopInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(mWidth, mHeight - (mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE)), i + mTopInputCount, QPointF(4, 0)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(0, mHeight - (mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE)), i, QPointF(-4, 0)));
            }
            break;
        }
        case Direction::UP:
        {
            for (uint8_t i = 0; i < mTopInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, mHeight - (canvas::GRID_SIZE * (mTopInputCount - i - 1) + canvas::GRID_SIZE)), i, QPointF(-4, 0)));
            }
            for (uint8_t i = 0; i < mInputCount - mTopInputCount; i++)
            {
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight), i + mTopInputCount, QPointF(0, 4)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0), i, QPointF(0, -4)));
            }
            break;
        }
        default:
        {
            throw std::logic_error("Direction invalid");
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
                throw std::logic_error("Direction invalid");
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

        if (mSmallerDescription)
        {
            auto font = pPainter->font();
            font.setPointSize(10);
            pPainter->setFont(font);
        }

        pPainter->drawText(boundingRect(), mComponentText, Qt::AlignHCenter | Qt::AlignVCenter);
    }

    if (levelOfDetail >= components::SMALL_TEXT_MIN_LOD)
    {
        pPainter->setPen(components::complex_logic::SMALL_TEXT_FONT_COLOR);
        pPainter->setFont(components::complex_logic::CONNECTOR_FONT);
        switch (mDirection)
        {
            case Direction::RIGHT:
            {
                for (size_t i = 0; i < mTopInputCount; i++)
                {
                    pPainter->drawText(QRect(canvas::GRID_SIZE * i + canvas::GRID_SIZE - 20, 0, 40, mHeight), mInputLabels[mTopInputCount - i - 1], Qt::AlignHCenter | Qt::AlignTop);
                }
                for (int i = 0; i < mInputCount - mTopInputCount; i++)
                {
                    pPainter->drawText(QRect(2, mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 6, mWidth - 4, 10), mInputLabels[i + mTopInputCount], Qt::AlignLeft | Qt::AlignVCenter);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(2, mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 6, mWidth - 4, 10), mOutputLabels[i], Qt::AlignRight | Qt::AlignVCenter);
                }
                break;
            }
            case Direction::DOWN:
            {
                for (size_t i = 0; i < mTopInputCount; i++)
                {
                    pPainter->drawText(QRect(2, canvas::GRID_SIZE * i + canvas::GRID_SIZE - 6, mWidth - 4, 10), mInputLabels[mTopInputCount - i - 1], Qt::AlignRight | Qt::AlignVCenter);
                }
                for (int i = 0; i < mInputCount - mTopInputCount; i++)
                {
                    pPainter->drawText(QRect(mWidth - mInputsSpacing * canvas::GRID_SIZE * i - canvas::GRID_SIZE - 20, 0, 40, mHeight), mInputLabels[i + mTopInputCount], Qt::AlignHCenter | Qt::AlignTop);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(mWidth - (mOutputsSpacing * canvas::GRID_SIZE * (mOutputCount - i - 1) + canvas::GRID_SIZE) - 20, 0, 40, mHeight), mOutputLabels[mOutputCount - i - 1], Qt::AlignHCenter | Qt::AlignBottom);
                }
                break;
            }
            case Direction::LEFT:
            {
                for (size_t i = 0; i < mTopInputCount; i++)
                {
                    pPainter->drawText(QRect(mWidth - (canvas::GRID_SIZE * i + canvas::GRID_SIZE) - 20, 0, 40, mHeight), mInputLabels[mTopInputCount - i - 1], Qt::AlignHCenter | Qt::AlignBottom);
                }
                for (int i = 0; i < mInputCount - mTopInputCount; i++)
                {
                    pPainter->drawText(QRect(2, mHeight - mInputsSpacing * canvas::GRID_SIZE * i - canvas::GRID_SIZE - 6, mWidth - 4, 10), mInputLabels[i + mTopInputCount], Qt::AlignRight | Qt::AlignVCenter);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(2, mHeight - (mOutputsSpacing * canvas::GRID_SIZE * (mOutputCount - i - 1) + canvas::GRID_SIZE) - 6, mWidth - 4, 10), mOutputLabels[mOutputCount - i - 1], Qt::AlignLeft | Qt::AlignVCenter);
                }
                break;
            }
            case Direction::UP:
            {
                for (size_t i = 0; i < mTopInputCount; i++)
                {
                    pPainter->drawText(QRect(2, mHeight - (canvas::GRID_SIZE * i + canvas::GRID_SIZE) - 6, mWidth - 4, 10), mInputLabels[mTopInputCount - i - 1], Qt::AlignLeft | Qt::AlignVCenter);
                }
                for (int i = 0; i < mInputCount - mTopInputCount; i++)
                {
                    pPainter->drawText(QRect(mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 20, 0, 40, mHeight), mInputLabels[i + mTopInputCount], Qt::AlignHCenter | Qt::AlignBottom);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 20, 0, 40, mHeight), mOutputLabels[i], Qt::AlignHCenter | Qt::AlignTop);
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
    // Draw top input connectors
    for (size_t i = 0; i < mTopInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(mTopInputCount - i - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(canvas::GRID_SIZE * i + canvas::GRID_SIZE, -8, canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0);
    }

    // Draw input connectors
    for (int i = 0; i < mInputCount - mTopInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(-8, mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0, mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth, mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mWidth + 8, mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw inversion circles
    for (size_t i = 0; i < mTopInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(mTopInputCount - i - 1))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(mTopInputCount - i - 1), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, -9, 8, 8);
        }
    }
    for (int i = 0; i < mInputCount - mTopInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i + mTopInputCount))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(-9, mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth + 1, mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsDown(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw top input connectors
    for (size_t i = 0; i < mTopInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(mTopInputCount - i - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth, canvas::GRID_SIZE * i + canvas::GRID_SIZE, mWidth + 8, canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw input connectors
    for (int i = 0; i < mInputCount - mTopInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth - mInputsSpacing * canvas::GRID_SIZE * i - canvas::GRID_SIZE, -8,
                           mWidth - mInputsSpacing * canvas::GRID_SIZE * i - canvas::GRID_SIZE, 0);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth - (mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE), mHeight, mWidth - (mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE), mHeight + 8);
    }

    // Draw inversion circles
    for (size_t i = 0; i < mTopInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(mTopInputCount - i - 1))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(mTopInputCount - i - 1), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth + 1, canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    for (int i = 0; i < mInputCount - mTopInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i + mTopInputCount))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth - mInputsSpacing * canvas::GRID_SIZE * i - canvas::GRID_SIZE - 4, -9, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(mOutputCount - i - 1))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(mOutputCount - i - 1), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth - (mOutputsSpacing * canvas::GRID_SIZE * (mOutputCount - i - 1) + canvas::GRID_SIZE) - 4, mHeight + 1, 8, 8);
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsLeft(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw top input connectors
    for (size_t i = 0; i < mTopInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight, canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight + 8);
    }

    // Draw input connectors
    for (int i = 0; i < mInputCount - mTopInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth, mHeight - mInputsSpacing * canvas::GRID_SIZE * i - canvas::GRID_SIZE,
                           mWidth + 8, mHeight - mInputsSpacing * canvas::GRID_SIZE * i - canvas::GRID_SIZE);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(-8, mHeight - (mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE), 0, mHeight - (mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE));
    }

    // Draw inversion circles
    for (size_t i = 0; i < mTopInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, mHeight + 1, 8, 8);
        }
    }
    for (int i = 0; i < mInputCount - mTopInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i + mTopInputCount))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth + 1, mHeight - mInputsSpacing * canvas::GRID_SIZE * i - canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(mOutputCount - i - 1))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(mOutputCount - i - 1), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(-9, mHeight - (mOutputsSpacing * canvas::GRID_SIZE * (mOutputCount - i - 1) + canvas::GRID_SIZE) - 4, 8, 8);
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsUp(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    // Draw top input connectors
    for (size_t i = 0; i < mTopInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(-8, canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0, canvas::GRID_SIZE * i + canvas::GRID_SIZE);
    }

    // Draw input connectors
    for (int i = 0; i < mInputCount - mTopInputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight, mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE, mHeight + 8);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE, -8, mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE, 0);
    }

    // Draw inversion circles
    for (size_t i = 0; i < mTopInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(-9, canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    for (int i = 0; i < mInputCount - mTopInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i + mTopInputCount))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mInputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, mHeight + 1, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mOutputsSpacing * canvas::GRID_SIZE * i + canvas::GRID_SIZE - 4, -9, 8, 8);
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
        return QRectF(-13, -13, mWidth + 26, mHeight + 26);
    }
    else
    {
        return QRectF(-13, -13, mWidth + 26, mHeight + 26);
    }
}
