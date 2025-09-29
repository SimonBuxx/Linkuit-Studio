#include "LogicWire.h"
#include "CoreLogic.h"
#include "Configuration.h"
#include "HelperFunctions.h"

LogicWire::LogicWire(const CoreLogic* pCoreLogic, WireDirection pDirection, uint32_t pLength):
    IBaseComponent(pCoreLogic, nullptr),
    mDirection(pDirection)
{
    Q_ASSERT(pCoreLogic);

    if (pCoreLogic->GetControlMode() == ControlMode::EDIT || pCoreLogic->GetControlMode() == ControlMode::COPY)
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }

    setZValue(components::zvalues::WIRE);

    if (mDirection == WireDirection::HORIZONTAL)
    {
        mHeight = components::wires::BOUNDING_RECT_SIZE;
        mWidth = pLength;
        mShape.addRect(-1, mHeight * -0.5f, mWidth + 2, mHeight);
    }
    else
    {
        mWidth = components::wires::BOUNDING_RECT_SIZE;
        mHeight = pLength;
        mShape.addRect(mWidth * -0.5f, -1, mWidth, mHeight + 2);
    }
}

LogicWire::LogicWire(const LogicWire& pObj, const CoreLogic* pCoreLogic):
    LogicWire(pCoreLogic, pObj.mDirection, pObj.GetLength())
{};

LogicWire::LogicWire(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    LogicWire(pCoreLogic, static_cast<WireDirection>(pJson["dir"].toInt()), pJson["length"].toInt())
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));
}

IBaseComponent* LogicWire::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new LogicWire(*this, pCoreLogic);
}

void LogicWire::ResetZValue()
{
    setZValue(components::zvalues::WIRE);
}

void LogicWire::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    QPen pen;
    if (mLogicCell != nullptr && std::static_pointer_cast<LogicWireCell>(mLogicCell)->GetOutputState() == LogicState::HIGH)
    {
        pen = QPen(components::wires::WIRE_HIGH_COLOR, components::wires::WIRE_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    else
    {
        pen = QPen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::wires::WIRE_LOW_COLOR,
                     components::wires::WIRE_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    pPainter->setPen(pen);

    if (mDirection == WireDirection::HORIZONTAL)
    {
        pPainter->drawLine(0, 0, mWidth, 0);
    }
    else
    {
        pPainter->drawLine(0, 0, 0, mHeight);
    }
}

void LogicWire::SetLength(uint32_t pLength)
{
    if (mDirection == WireDirection::HORIZONTAL)
    {
        mWidth = pLength;
    }
    else
    {
        mHeight = pLength;
    }
}

uint32_t LogicWire::GetLength() const
{
    if (mDirection == WireDirection::HORIZONTAL)
    {
        return mWidth;
    }
    else
    {
        return mHeight;
    }
}

WireDirection LogicWire::GetDirection() const
{
    return mDirection;
}

bool LogicWire::StartsOrEndsIn(QPointF pPoint) const
{
    return ((mDirection == WireDirection::HORIZONTAL && QPointF(x() + mWidth, y()) == pPoint) ||
            (mDirection == WireDirection::VERTICAL && QPointF(x(), y() + mHeight) == pPoint) ||
            (pos() == pPoint));
}

void LogicWire::SetLogicCell(const std::shared_ptr<LogicWireCell>& pLogicCell)
{
    mLogicCell = pLogicCell;

    QObject::connect(mLogicCell.get(), &LogicBaseCell::StateChangedSignal, this, &LogicWire::OnLogicStateChanged);
}

QRectF LogicWire::boundingRect() const
{
    if (mDirection == WireDirection::HORIZONTAL)
    {
        return QRectF(-2, mHeight * -0.5f, mWidth + 4, mHeight);
    }
    else
    {
        return QRectF(mWidth * -0.5f, -2, mWidth, mHeight + 4);
    }
}

QJsonObject LogicWire::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::WIRE;
    json["x"] = x();
    json["y"] = y();
    if (mDirection == WireDirection::HORIZONTAL)
    {
        json["length"] = static_cast<int32_t>(mWidth);
    }
    else
    {
        json["length"] = static_cast<int32_t>(mHeight);
    }
    json["dir"] = static_cast<int32_t>(mDirection);

    return json;
}

SwVersion LogicWire::GetMinVersion(void) const
{
    return SwVersion(0, 0, 0);
}
