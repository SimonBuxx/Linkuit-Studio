#include "AbstractComplexLogic.h"
#include "Configuration.h"

AbstractComplexLogic::AbstractComplexLogic(const CoreLogic* pCoreLogic, const std::shared_ptr<LogicBaseCell>& pLogicCell, uint8_t pInputCount, uint8_t pOutputCount,
                                           Direction pDirection, uint8_t pTopInputCount, bool pStrechTwoPins, bool pTrapezoidShape):
    IBaseComponent(pCoreLogic, pLogicCell),
    mInputCount(pInputCount),
    mOutputCount(pOutputCount),
    mDirection(pDirection),
    mInputsSpacing(1),
    mOutputsSpacing(1),
    mTopInputCount(pTopInputCount),
    mSmallerDescription(false),
    mTrapezoidShape(pTrapezoidShape),
    mInputsTrapezoidOffset(0),
    mOutputsTrapezoidOffset(0)
{
    Q_ASSERT(!mTrapezoidShape || abs(mInputCount - mOutputCount) >= 4); // Assert that the in- and output count fits the trapezoid shape
    Q_ASSERT(!mTrapezoidShape || mTopInputCount == 0);

    setZValue(components::zvalues::GATE);

    if (pStrechTwoPins) // Set in- and output spacing
    {
        if (mInputCount - mTopInputCount == 2)
        {
            mInputsSpacing = 2;
        }

        if (mOutputCount == 2)
        {
            mOutputsSpacing = 2;
        }
    }

    if (mTrapezoidShape)
    {
        if (mInputCount < mOutputCount)
        {
            mInputsTrapezoidOffset = std::floor((mOutputCount - mInputCount) / 2);
        }
        else // cannot be equal because of assertion above
        {
            mOutputsTrapezoidOffset = std::floor((mInputCount - mOutputCount) / 2);

        }
    }

    if (mDirection == Direction::RIGHT || mDirection == Direction::LEFT) // Note: Base dimensions are equal for rect and trapezoid
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

#warning [ENHANCEMENT] refactor
    if (mTrapezoidShape)
    {
        switch (mDirection)
        {
            case Direction::RIGHT:
            {
                if (mInputCount < mOutputCount)
                {
                    mTrapezoid << QPoint(0, 2 * canvas::GRID_SIZE) << QPoint(mWidth, 0) << QPoint(mWidth, mHeight) << QPoint(0, mHeight - 2 * canvas::GRID_SIZE);
                }
                else
                {
                    mTrapezoid << QPoint(0, 0) << QPoint(mWidth, 2 * canvas::GRID_SIZE) << QPoint(mWidth, mHeight - 2 * canvas::GRID_SIZE) << QPoint(0, mHeight);
                }
                break;
            }
            case Direction::DOWN:
            {
                if (mInputCount < mOutputCount)
                {
                    mTrapezoid << QPoint(2 * canvas::GRID_SIZE, 0) << QPoint(mWidth - 2 * canvas::GRID_SIZE, 0) << QPoint(mWidth, mHeight) << QPoint(0, mHeight);
                }
                else
                {
                    mTrapezoid << QPoint(0, 0) << QPoint(mWidth, 0) << QPoint(mWidth - 2 * canvas::GRID_SIZE, mHeight) << QPoint(2 * canvas::GRID_SIZE, mHeight);
                }
                break;
            }
            case Direction::LEFT:
            {
                if (mInputCount < mOutputCount)
                {
                    mTrapezoid << QPoint(0, 0) << QPoint(mWidth, 2 * canvas::GRID_SIZE) << QPoint(mWidth, mHeight - 2 * canvas::GRID_SIZE) << QPoint(0, mHeight);
                }
                else
                {
                    mTrapezoid << QPoint(0, 2 * canvas::GRID_SIZE) << QPoint(mWidth, 0) << QPoint(mWidth, mHeight) << QPoint(0, mHeight - 2 * canvas::GRID_SIZE);
                }
                break;
            }
            case Direction::UP:
            {
                if (mInputCount < mOutputCount)
                {
                    mTrapezoid << QPoint(0, 0) << QPoint(mWidth, 0) << QPoint(mWidth - 2 * canvas::GRID_SIZE, mHeight) << QPoint(2 * canvas::GRID_SIZE, mHeight);
                }
                else
                {
                    mTrapezoid << QPoint(2 * canvas::GRID_SIZE, 0) << QPoint(mWidth - 2 * canvas::GRID_SIZE, 0) << QPoint(mWidth, mHeight) << QPoint(0, mHeight);
                }
                break;
            }
            default:
            {
                throw std::logic_error("Invalid direction");
            }
        }

        mShape.addPolygon(mTrapezoid);
    }
    else
    {
        mShape.addRect(0, 0, mWidth, mHeight);
    }

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
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE), i + mTopInputCount, QPointF(-4, 0)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth, (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE), i, QPointF(4, 0)));
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
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(mWidth - (canvas::GRID_SIZE * (mInputsSpacing * i + mInputsTrapezoidOffset + 1)), 0), i + mTopInputCount, QPointF(0, -4)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(mWidth - (canvas::GRID_SIZE * (mInputsSpacing * i + mOutputsTrapezoidOffset + 1)), mHeight), i, QPointF(0, 4)));
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
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(mWidth, mHeight - (canvas::GRID_SIZE * (mInputsSpacing * i + mInputsTrapezoidOffset + 1))), i + mTopInputCount, QPointF(4, 0)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(0, mHeight - (canvas::GRID_SIZE * (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1))), i, QPointF(-4, 0)));
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
                mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(canvas::GRID_SIZE * (mInputsSpacing * i + mInputsTrapezoidOffset + 1), mHeight), i + mTopInputCount, QPointF(0, 4)));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(canvas::GRID_SIZE * (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1), 0), i, QPointF(0, -4)));
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
    Q_ASSERT(pPainter);
    Q_ASSERT(pItem);
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
    if (!mTrapezoidShape)
    {
        QPen pen(pItem->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR,
                 components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::FILL_COLOR));
        pPainter->drawRect(0, 0, mWidth, mHeight);
    }
    else
    {
        QPen pen(pItem->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR,
                 components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::FILL_COLOR));

        pPainter->drawPolygon(mTrapezoid);
    }

    // Draw description text
    if (levelOfDetail >= components::DESCRIPTION_TEXT_MIN_LOD)
    {
        pPainter->setPen(components::complex_logic::FONT_COLOR);
        pPainter->setFont(components::complex_logic::FONT);

        if (mSmallerDescription)
        {
            auto font = pPainter->font();
            font.setPointSize(8);
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
#warning split into sub functions
            case Direction::RIGHT:
            {
                for (size_t i = 0; i < mTopInputCount; i++)
                {
                    if (mInputLabels[mTopInputCount - i - 1] == ">")
                    {
                        SetClockInputPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
                        const QList<QPoint> trianglePointsDown = {QPoint((i + 1) * canvas::GRID_SIZE - 5, 0),
                                                                  QPoint((i + 1) * canvas::GRID_SIZE + 5, 0),
                                                                  QPoint((i + 1) * canvas::GRID_SIZE, 8)};
                        pPainter->drawConvexPolygon(QPolygon(trianglePointsDown));
                    }
                    else
                    {
                        pPainter->setPen(components::complex_logic::SMALL_TEXT_FONT_COLOR);
                        pPainter->drawText(QRect(canvas::GRID_SIZE * i + canvas::GRID_SIZE - 20, 0, 40, mHeight), mInputLabels[mTopInputCount - i - 1], Qt::AlignHCenter | Qt::AlignTop);
                    }
                }
                for (int i = 0; i < mInputCount - mTopInputCount; i++)
                {
                    if (mInputLabels[i + mTopInputCount] == ">")
                    {
                        SetClockInputPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
                        const QList<QPoint> trianglePointsRight = {QPoint(0, (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 5),
                                                                   QPoint(0, (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE + 5),
                                                                   QPoint(8, (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE)};
                        pPainter->drawConvexPolygon(QPolygon(trianglePointsRight));
                    }
                    else
                    {
                        pPainter->setPen(components::complex_logic::SMALL_TEXT_FONT_COLOR);
                        pPainter->drawText(QRect(2, (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 6, mWidth - 4, 10), mInputLabels[i + mTopInputCount], Qt::AlignLeft | Qt::AlignVCenter);
                    }
                }
                pPainter->setPen(components::complex_logic::SMALL_TEXT_FONT_COLOR);
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(2, (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 6, mWidth - 4, 10), mOutputLabels[i], Qt::AlignRight | Qt::AlignVCenter);
                }
                break;
            }
            case Direction::DOWN:
            {
                for (size_t i = 0; i < mTopInputCount; i++)
                {
                    if (mInputLabels[mTopInputCount - i - 1] == ">")
                    {
                        SetClockInputPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
                        const QList<QPoint> trianglePointsLeft = {QPoint(mWidth, canvas::GRID_SIZE * (i + 1) - 5), QPoint(mWidth, canvas::GRID_SIZE * (i + 1) + 5), QPoint(mWidth - 8, canvas::GRID_SIZE * (i + 1))};
                        pPainter->drawConvexPolygon(QPolygon(trianglePointsLeft));
                    }
                    else
                    {
                        pPainter->setPen(components::complex_logic::SMALL_TEXT_FONT_COLOR);
                        pPainter->drawText(QRect(2, canvas::GRID_SIZE * i + canvas::GRID_SIZE - 6, mWidth - 4, 10), mInputLabels[mTopInputCount - i - 1], Qt::AlignRight | Qt::AlignVCenter);
                    }
                }
                for (int i = 0; i < mInputCount - mTopInputCount; i++)
                {
                    if (mInputLabels[i + mTopInputCount] == ">")
                    {
                        SetClockInputPen(pPainter, mLogicCell->GetInputState(i + mTopInputCount), pItem->state & QStyle::State_Selected);
                        const QList<QPoint> trianglePointsDown = {QPoint(mWidth - (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 5, 0),
                                                                         QPoint(mWidth - (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE + 5, 0),
                                                                         QPoint(mWidth - (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE, 8)};
                        pPainter->drawConvexPolygon(QPolygon(trianglePointsDown));
                    }
                    else
                    {
                        pPainter->setPen(components::complex_logic::SMALL_TEXT_FONT_COLOR);
                        pPainter->drawText(QRect(mWidth - (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 20, 0, 40, mHeight), mInputLabels[i + mTopInputCount], Qt::AlignHCenter | Qt::AlignTop);
                    }
                }
                pPainter->setPen(components::complex_logic::SMALL_TEXT_FONT_COLOR);
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(mWidth - (mOutputsSpacing * canvas::GRID_SIZE * (mOutputCount - i - 1) + canvas::GRID_SIZE * (mOutputsTrapezoidOffset + 1)) - 20, 0, 40, mHeight), mOutputLabels[mOutputCount - i - 1], Qt::AlignHCenter | Qt::AlignBottom);
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
                    pPainter->drawText(QRect(2, mHeight - (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 6, mWidth - 4, 10), mInputLabels[i + mTopInputCount], Qt::AlignRight | Qt::AlignVCenter);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect(2, mHeight - (mOutputsSpacing * canvas::GRID_SIZE * (mOutputCount - i - 1) + canvas::GRID_SIZE * (mOutputsTrapezoidOffset + 1)) - 6, mWidth - 4, 10), mOutputLabels[mOutputCount - i - 1], Qt::AlignLeft | Qt::AlignVCenter);
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
                    pPainter->drawText(QRect((mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 20, 0, 40, mHeight), mInputLabels[i + mTopInputCount], Qt::AlignHCenter | Qt::AlignBottom);
                }
                for (size_t i = 0; i < mOutputCount; i++)
                {
                    pPainter->drawText(QRect((mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 20, 0, 40, mHeight), mOutputLabels[i], Qt::AlignHCenter | Qt::AlignTop);
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
    Q_ASSERT(pPainter);
    Q_ASSERT(pItem);

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
        pPainter->drawLine(-8, (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE, 0, (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth, (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE, mWidth + 8, (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE);
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
            pPainter->drawEllipse(-9, (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth + 1, (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 4, 8, 8);
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsDown(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    Q_ASSERT(pPainter);
    Q_ASSERT(pItem);

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
        pPainter->drawLine(mWidth - canvas::GRID_SIZE * (mInputsSpacing * i + mInputsTrapezoidOffset + 1), -8,
                           mWidth - canvas::GRID_SIZE * (mInputsSpacing * i + mInputsTrapezoidOffset + 1), 0);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth - (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE, mHeight,
                           mWidth - (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE, mHeight + 8);
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
            pPainter->drawEllipse(mWidth - (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 4, -9, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(mOutputCount - i - 1))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(mOutputCount - i - 1), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth - (mOutputsSpacing * canvas::GRID_SIZE * (mOutputCount - i - 1) + canvas::GRID_SIZE * (mOutputsTrapezoidOffset + 1)) - 4, mHeight + 1, 8, 8);
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsLeft(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    Q_ASSERT(pPainter);
    Q_ASSERT(pItem);

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
        pPainter->drawLine(mWidth, mHeight - (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE,
                           mWidth + 8, mHeight - (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(-8, mHeight - ((mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE), 0,
                               mHeight - ((mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE));
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
            pPainter->drawEllipse(mWidth + 1, mHeight - (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(mOutputCount - i - 1))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(mOutputCount - i - 1), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(-9, mHeight - (mOutputsSpacing * canvas::GRID_SIZE * (mOutputCount - i - 1) + canvas::GRID_SIZE * (mOutputsTrapezoidOffset + 1)) - 4, 8, 8);
        }
    }
}

void AbstractComplexLogic::DrawComponentDetailsUp(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    Q_ASSERT(pPainter);
    Q_ASSERT(pItem);

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
        pPainter->drawLine((mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE, mHeight, (mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE, mHeight + 8);
    }

    // Draw output connectors
    for (size_t i = 0; i < mOutputCount; i++)
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
        pPainter->drawLine((mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE, -8, (mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE, 0);
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
            pPainter->drawEllipse((mInputsSpacing * i + mInputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 4, mHeight + 1, 8, 8);
        }
    }
    for (size_t i = 0; i < mOutputCount; i++)
    {
        if (mLogicCell->IsOutputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetOutputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse((mOutputsSpacing * i + mOutputsTrapezoidOffset + 1) * canvas::GRID_SIZE - 4, -9, 8, 8);
        }
    }
}

void AbstractComplexLogic::SetConnectorPen(QPainter *pPainter, LogicState pState, bool pSelected)
{
    Q_ASSERT(pPainter);

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
    Q_ASSERT(pPainter);

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

void AbstractComplexLogic::SetClockInputPen(QPainter *pPainter, LogicState pState, bool pSelected)
{
    Q_ASSERT(pPainter);

    if (pState == LogicState::HIGH)
    {
        pPainter->setPen(QPen(components::HIGH_COLOR, components::wires::WIRE_WIDTH, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        pPainter->setBrush(components::HIGH_COLOR);
    }
    else
    {
        pPainter->setPen(QPen(pSelected ? components::SELECTED_BORDER_COLOR : components::wires::WIRE_LOW_COLOR,
                              components::wires::WIRE_WIDTH, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
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
