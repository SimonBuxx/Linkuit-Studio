#include "ShiftRegister.h"
#include "CoreLogic.h"
#include "LogicCells/LogicShiftRegisterCell.h"

static constexpr QPointF UNUSED_CONNECTORS_POS{2147483647, 2147483647};

ShiftRegister::ShiftRegister(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pBitWidth):
    AbstractComplexLogic(pCoreLogic, std::make_shared<LogicShiftRegisterCell>(pBitWidth), 2, pBitWidth, pDirection, 0, false),
    mBitWidth(pBitWidth)
{
    Q_ASSERT(pBitWidth <= 32);
    mComponentText = "";


    mInputLabels.push_back("");
    mInputLabels.push_back("");

    for (int i = 0; i < pBitWidth; i++)
    {
        mOutputLabels.push_back("");
    }

    if (mDirection == Direction::RIGHT || mDirection == Direction::LEFT)
    {
        mWidth = (mBitWidth + 1) * canvas::GRID_SIZE;
        mHeight = canvas::GRID_SIZE * 3;
    }
    else
    {
        mWidth = canvas::GRID_SIZE * 3;
        mHeight = (mBitWidth + 1) * canvas::GRID_SIZE;
    }

    mShape.clear();
    mShape.addRect(0, 0, mWidth, mHeight);

#warning ShiftRegister should inherit IBaseComponent directly, not AbstractComplexLogic

    SetLogicConnectors();
}

ShiftRegister::ShiftRegister(const ShiftRegister& pObj, const CoreLogic* pCoreLogic):
    ShiftRegister(pCoreLogic, pObj.mDirection, pObj.mBitWidth)
{
    mLogicCell->SetInputInversions(pObj.mLogicCell->GetInputInversions());
    mLogicCell->SetOutputInversions(pObj.mLogicCell->GetOutputInversions());
};

ShiftRegister::ShiftRegister(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    ShiftRegister(pCoreLogic, static_cast<Direction>(pJson["dir"].toInt()), pJson["bits"].toInt())
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));

    std::vector<bool> ininv, outinv;
    auto ininvArray = pJson["ininv"].toArray();
    for (const auto& inv : ininvArray)
    {
        ininv.push_back(inv.toBool());
    }
    GetLogicCell()->SetInputInversions(ininv);

    auto outinvArray = pJson["outinv"].toArray();
    for (const auto& inv : outinvArray)
    {
        outinv.push_back(inv.toBool());
    }
    GetLogicCell()->SetOutputInversions(outinv);
}

IBaseComponent* ShiftRegister::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new ShiftRegister(*this, pCoreLogic);
}

QJsonObject ShiftRegister::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::SHIFTREGISTER;
    json["x"] = x();
    json["y"] = y();
    json["dir"] = static_cast<int32_t>(mDirection);
    json["bits"] = mBitWidth;
    {
        QJsonArray ininv, outinv;

        for(const bool& inv : mLogicCell->GetInputInversions())
        {
            ininv.append(inv);
        }

        json["ininv"] = ininv;

        for(const bool& inv : mLogicCell->GetOutputInversions())
        {
            outinv.append(inv);
        }

        json["outinv"] = outinv;
    }

    return json;
}

SwVersion ShiftRegister::GetMinVersion(void) const
{
    return SwVersion(0, 9, 7);
}

void ShiftRegister::SetLogicConnectors()
{
    switch(mDirection)
    {
        case Direction::RIGHT:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors[i] = LogicConnector(ConnectorType::IN, QPointF(0, (mInputsSpacing * i + 1) * canvas::GRID_SIZE), i, QPointF(-4, 0));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors[i].pos = UNUSED_CONNECTORS_POS;
            }
            mOutConnectors[mOutputCount - 1] = LogicConnector(ConnectorType::OUT, QPointF(mWidth, canvas::GRID_SIZE), mBitWidth - 1, QPointF(4, 0));
            break;
        }
        case Direction::DOWN:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors[i] = LogicConnector(ConnectorType::IN, QPointF(canvas::GRID_SIZE * (mInputsSpacing * i + 1), 0), i, QPointF(0, -4));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors[i].pos = UNUSED_CONNECTORS_POS;
            }
            mOutConnectors[mOutputCount - 1] = LogicConnector(ConnectorType::OUT, QPointF(mWidth - canvas::GRID_SIZE, mHeight), mBitWidth - 1, QPointF(0, 4));
            break;
        }
        case Direction::LEFT:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors[i] = LogicConnector(ConnectorType::IN, QPointF(mWidth, mHeight - (canvas::GRID_SIZE * (mInputsSpacing * i + 1))), i, QPointF(4, 0));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors[i].pos = UNUSED_CONNECTORS_POS;
            }
            mOutConnectors[mOutputCount - 1] = LogicConnector(ConnectorType::OUT, QPointF(0, mHeight - canvas::GRID_SIZE), mBitWidth - 1, QPointF(-4, 0));
            break;
        }
        case Direction::UP:
        {
            for (uint8_t i = 0; i < mInputCount; i++)
            {
                mInConnectors[i] = LogicConnector(ConnectorType::IN, QPointF(canvas::GRID_SIZE * (mInputsSpacing * i + 1), mHeight), i, QPointF(0, 4));
            }
            for (uint8_t i = 0; i < mOutputCount; i++)
            {
                mOutConnectors[i].pos = UNUSED_CONNECTORS_POS;
            }
            mOutConnectors[mOutputCount - 1] = LogicConnector(ConnectorType::OUT, QPointF(canvas::GRID_SIZE, 0), mBitWidth - 1, QPointF(0, -4));
            break;
        }
        default:
        {
            throw std::logic_error("Direction invalid");
        }
    }
}

void ShiftRegister::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget)
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
    const QPen bodyPen(pItem->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR,
             components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pPainter->setPen(bodyPen);
    pPainter->setBrush(QBrush(components::FILL_COLOR));
    pPainter->drawRect(0, 0, mWidth, mHeight);


    // Draw bit cells
    const auto bitStretch = canvas::GRID_SIZE - components::BORDER_WIDTH - 2; // Width or height of one bit cell

    switch (mDirection)
    {
        case Direction::RIGHT:
        {

            for (size_t i = 0; i < mBitWidth; i++)
            {
                pPainter->setBrush(QBrush(std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(i) == LogicState::HIGH ? components::HIGH_COLOR : components::wires::WIRE_LOW_COLOR));
                pPainter->setPen(QPen(std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(i) == LogicState::HIGH ? components::HIGH_COLOR : components::wires::WIRE_LOW_COLOR,
                                      components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

                const auto x = (i + 1) * canvas::GRID_SIZE - components::BORDER_WIDTH + 1;
                pPainter->drawRect(x, 2 * components::BORDER_WIDTH, bitStretch, 3 * canvas::GRID_SIZE - 4 * components::BORDER_WIDTH);
            }
            break;
        }
        case Direction::DOWN:
        {
            for (size_t i = 0; i < mBitWidth; i++)
            {
                pPainter->setBrush(QBrush(std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(i) == LogicState::HIGH ? components::HIGH_COLOR : components::wires::WIRE_LOW_COLOR));
                pPainter->setPen(QPen(std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(i) == LogicState::HIGH ? components::HIGH_COLOR : components::wires::WIRE_LOW_COLOR,
                                      components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

                const auto y = (i + 1) * canvas::GRID_SIZE - components::BORDER_WIDTH + 1;
                pPainter->drawRect(2 * components::BORDER_WIDTH, y, 3 * canvas::GRID_SIZE - 4 * components::BORDER_WIDTH, bitStretch);
            }
            break;
        }
        case Direction::LEFT:
        {
            for (size_t i = 0; i < mBitWidth; i++)
            {
                pPainter->setBrush(QBrush(std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(mBitWidth - i - 1) == LogicState::HIGH ? components::HIGH_COLOR : components::wires::WIRE_LOW_COLOR));
                pPainter->setPen(QPen(std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(mBitWidth - i - 1) == LogicState::HIGH ? components::HIGH_COLOR : components::wires::WIRE_LOW_COLOR,
                                      components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

                const auto x = i * canvas::GRID_SIZE + 2 * components::BORDER_WIDTH + 1;
                pPainter->drawRect(x, 2 * components::BORDER_WIDTH, bitStretch, 3 * canvas::GRID_SIZE - 4 * components::BORDER_WIDTH);
            }
            break;
        }
        case Direction::UP:
        {
            for (size_t i = 0; i < mBitWidth; i++)
            {
                pPainter->setBrush(QBrush(std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(mBitWidth - i - 1) == LogicState::HIGH ? components::HIGH_COLOR : components::wires::WIRE_LOW_COLOR));
                pPainter->setPen(QPen(std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(mBitWidth - i - 1) == LogicState::HIGH ? components::HIGH_COLOR : components::wires::WIRE_LOW_COLOR,
                                      components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

                const auto y = i * canvas::GRID_SIZE + 2 * components::BORDER_WIDTH + 1;
                pPainter->drawRect(2 * components::BORDER_WIDTH, y, 3 * canvas::GRID_SIZE - 4 * components::BORDER_WIDTH, bitStretch);
            }
            break;
        }
        default:
        {
            throw std::logic_error("Direction invalid");
        }
    }

    // Draw clock input triangle
    if (levelOfDetail >= components::SMALL_TEXT_MIN_LOD)
    {
        SetClockInputPen(pPainter, mLogicCell->GetInputState(1), pItem->state & QStyle::State_Selected);

        switch (mDirection)
        {
            case Direction::RIGHT:
            {
                static const QList<QPoint> trianglePointsRight = {QPoint(0, 2 * canvas::GRID_SIZE - 5), QPoint(0, 2 * canvas::GRID_SIZE + 5), QPoint(8, 2 * canvas::GRID_SIZE)};
                pPainter->drawConvexPolygon(QPolygon(trianglePointsRight));
                break;
            }
            case Direction::DOWN:
            {
                static const QList<QPoint> trianglePointsDown = {QPoint(2 * canvas::GRID_SIZE - 5, 0), QPoint(2 * canvas::GRID_SIZE + 5, 0), QPoint(2 * canvas::GRID_SIZE, 8)};
                pPainter->drawConvexPolygon(QPolygon(trianglePointsDown));
                break;
            }
            case Direction::LEFT:
            {
                const QList<QPoint> trianglePointsLeft = {QPoint(mWidth, canvas::GRID_SIZE - 5), QPoint(mWidth, canvas::GRID_SIZE + 5), QPoint(mWidth - 8, canvas::GRID_SIZE)};
                pPainter->drawConvexPolygon(QPolygon(trianglePointsLeft));
                break;
            }
            case Direction::UP:
            {
                const QList<QPoint> trianglePointsUp = {QPoint(2 * canvas::GRID_SIZE - 5, mHeight), QPoint(2 * canvas::GRID_SIZE + 5, mHeight), QPoint(2 * canvas::GRID_SIZE, mHeight - 8)};
                pPainter->drawConvexPolygon(QPolygon(trianglePointsUp));
                break;
            }
            default:
            {
                throw std::logic_error("Direction invalid");
            }
        }

        // Draw state text
        pPainter->setFont(components::shift_register::FONT);
        switch (mDirection)
        {
            case Direction::RIGHT:
            {
                for (int i = 0; i < mOutputCount; i++)
                {
                    const auto state = std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(i);
                    const auto text = (state == LogicState::HIGH) ? "1" : "0";
                    const auto x = (i + 1) * canvas::GRID_SIZE - components::BORDER_WIDTH + 1;

                    pPainter->setPen((state == LogicState::HIGH) ? components::shift_register::HIGH_FONT_COLOR: components::shift_register::LOW_FONT_COLOR);
                    pPainter->drawText(QRect(x, 2 * components::BORDER_WIDTH, bitStretch, mHeight - 4 * components::BORDER_WIDTH), text, Qt::AlignHCenter | Qt::AlignBottom);
                }
                break;
            }
            case Direction::DOWN:
            {
                for (int i = 0; i < mOutputCount; i++)
                {
                    const auto state = std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(i);
                    const auto text = (state == LogicState::HIGH) ? "1" : "0";
                    const auto y = (i + 1) * canvas::GRID_SIZE - components::BORDER_WIDTH + 1;

                    pPainter->setPen((state == LogicState::HIGH) ? components::shift_register::HIGH_FONT_COLOR: components::shift_register::LOW_FONT_COLOR);
                    pPainter->drawText(QRect(4 * components::BORDER_WIDTH, y, mWidth - 8 * components::BORDER_WIDTH, bitStretch), text, Qt::AlignLeft | Qt::AlignVCenter);
                }
                break;
            }
            case Direction::LEFT:
            {
                for (int i = 0; i < mOutputCount; i++)
                {
                    const auto state = std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(mOutputCount - i - 1);
                    const auto text = (state == LogicState::HIGH) ? "1" : "0";
                    const auto x = i * canvas::GRID_SIZE + 2 * components::BORDER_WIDTH + 1;

                    pPainter->setPen((state == LogicState::HIGH) ? components::shift_register::HIGH_FONT_COLOR: components::shift_register::LOW_FONT_COLOR);
                    pPainter->drawText(QRect(x, 2 * components::BORDER_WIDTH, bitStretch, mHeight - 4 * components::BORDER_WIDTH), text, Qt::AlignHCenter | Qt::AlignTop);
                }
                break;
            }
            case Direction::UP:
            {   
                for (int i = 0; i < mOutputCount; i++)
                {
                    const auto state = std::static_pointer_cast<LogicShiftRegisterCell>(mLogicCell)->GetOutputStateUninverted(mOutputCount - i - 1);
                    const auto text = (state == LogicState::HIGH) ? "1" : "0";
                    const auto y = i * canvas::GRID_SIZE + 2 * components::BORDER_WIDTH + 1;

                    pPainter->setPen((state == LogicState::HIGH) ? components::shift_register::HIGH_FONT_COLOR: components::shift_register::LOW_FONT_COLOR);
                    pPainter->drawText(QRect(4 * components::BORDER_WIDTH, y, mWidth - 8 * components::BORDER_WIDTH, bitStretch), text, Qt::AlignRight | Qt::AlignVCenter);
                }
                break;
            }
            default:
            {
                throw std::logic_error("Direction invalid");
            }
        }
    }
}

void ShiftRegister::DrawComponentDetailsRight(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    Q_ASSERT(pPainter);
    Q_ASSERT(pItem);

    // Draw input connectors
    for (int i = 0; i < mInputCount; i++)
    {
        if (!mLogicCell->IsInputConnected(i))
        {
            SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawLine(-8, (mInputsSpacing * i + 1) * canvas::GRID_SIZE, 0, (mInputsSpacing * i + 1) * canvas::GRID_SIZE);
        }
    }

    // Draw output connector
    if (!mLogicCell->IsOutputConnected(0))
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(mOutputCount - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth, canvas::GRID_SIZE, mWidth + 8, canvas::GRID_SIZE);
    }

    // Draw inversion circles
    for (int i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(-9, (mInputsSpacing * i + 1) * canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    if (mLogicCell->IsOutputInverted(mOutputCount - 1))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(mOutputCount - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawEllipse(mWidth + 1, canvas::GRID_SIZE - 4, 8, 8);
    }
}

void ShiftRegister::DrawComponentDetailsDown(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    Q_ASSERT(pPainter);
    Q_ASSERT(pItem);

    // Draw input connectors
    for (int i = 0; i < mInputCount; i++)
    {
        if (!mLogicCell->IsInputConnected(i))
        {
            SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawLine(canvas::GRID_SIZE * (mInputsSpacing * i + 1), -8,
                               canvas::GRID_SIZE * (mInputsSpacing * i + 1), 0);
        }
    }

    // Draw output connector
    if (!mLogicCell->IsOutputConnected(0))
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(mOutputCount - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(mWidth - canvas::GRID_SIZE, mHeight,
                           mWidth - canvas::GRID_SIZE, mHeight + 8);
    }

    // Draw inversion circles
    for (int i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth - (mInputsSpacing * i + 1) * canvas::GRID_SIZE - 4, -9, 8, 8);
        }
    }
    if (mLogicCell->IsOutputInverted(mOutputCount - 1))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(mOutputCount - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawEllipse(mWidth - canvas::GRID_SIZE - 4, mHeight + 1, 8, 8);
    }
}

void ShiftRegister::DrawComponentDetailsLeft(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    Q_ASSERT(pPainter);
    Q_ASSERT(pItem);

    // Draw input connectors
    for (int i = 0; i < mInputCount; i++)
    {
        if (!mLogicCell->IsInputConnected(i))
        {
            SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawLine(mWidth, mHeight - (mInputsSpacing * i + 1) * canvas::GRID_SIZE,
                               mWidth + 8, mHeight - (mInputsSpacing * i + 1) * canvas::GRID_SIZE);
        }
    }

    // Draw output connector
    if (!mLogicCell->IsOutputConnected(0))
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(mOutputCount - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(-8, mHeight - canvas::GRID_SIZE, 0,
                                   mHeight - canvas::GRID_SIZE);
    }

    // Draw inversion circles
    for (int i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse(mWidth + 1, mHeight - (mInputsSpacing * i + 1) * canvas::GRID_SIZE - 4, 8, 8);
        }
    }
    if (mLogicCell->IsOutputInverted(mOutputCount - 1))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(mOutputCount - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawEllipse(-9, mHeight - canvas::GRID_SIZE - 4, 8, 8);
    }
}

void ShiftRegister::DrawComponentDetailsUp(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem)
{
    Q_ASSERT(pPainter);
    Q_ASSERT(pItem);

    // Draw input connectors
    for (int i = 0; i < mInputCount; i++)
    {
        if (!mLogicCell->IsInputConnected(i))
        {
            SetConnectorPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawLine((mInputsSpacing * i + 1) * canvas::GRID_SIZE, mHeight, (mInputsSpacing * i + 1) * canvas::GRID_SIZE, mHeight + 8);
        }
    }

    // Draw output connector
    if (!mLogicCell->IsOutputConnected(0))
    {
        SetConnectorPen(pPainter, mLogicCell->GetOutputState(mOutputCount - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawLine(canvas::GRID_SIZE, -8, canvas::GRID_SIZE, 0);
    }

    // Draw inversion circles
    for (int i = 0; i < mInputCount; i++)
    {
        if (mLogicCell->IsInputInverted(i))
        {
            SetInversionPen(pPainter, mLogicCell->GetInputState(i), pItem->state & QStyle::State_Selected);
            pPainter->drawEllipse((mInputsSpacing * i + 1) * canvas::GRID_SIZE - 4, mHeight + 1, 8, 8);
        }
    }
    if (mLogicCell->IsOutputInverted(mOutputCount - 1))
    {
        SetInversionPen(pPainter, mLogicCell->GetOutputState(mOutputCount - 1), pItem->state & QStyle::State_Selected);
        pPainter->drawEllipse(canvas::GRID_SIZE - 4, -9, 8, 8);
    }
}

QRectF ShiftRegister::boundingRect() const
{
    return QRectF(-13, -13, mWidth + 26, mHeight + 26);
}
