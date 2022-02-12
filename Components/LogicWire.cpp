#include "LogicWire.h"
#include "Configuration.h"

LogicWire::LogicWire(WireDirection pDirection, uint32_t pLength):
    mDirection(pDirection),
    mState(LogicState::LOW)
{
    setZValue(components::zvalues::WIRE);

    if (mDirection == WireDirection::HORIZONTAL)
    {
        mHeight = components::wires::BOUNDING_RECT_SIZE;
        mWidth = pLength;
    }
    else
    {
        mWidth = components::wires::BOUNDING_RECT_SIZE;
        mHeight = pLength;
    }
}

LogicWire::LogicWire(const LogicWire& pObj)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mDirection = pObj.mDirection;
    mState = pObj.mState;
};

BaseComponent* LogicWire::CloneBaseComponent() const
{
    return new LogicWire(*this);
}

void LogicWire::ResetZValue()
{
    setZValue(components::zvalues::WIRE);
}

void LogicWire::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    QPen pen;
    if (mState == LogicState::LOW)
    {
        pen = QPen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::BORDER_COLOR,
                     components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    else if (mState == LogicState::HIGH)
    {
        pen = QPen(Qt::white, components::BORDER_WIDTH + 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
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

uint32_t LogicWire::GetLength(void) const
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

WireDirection LogicWire::GetDirection(void) const
{
    return mDirection;
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

QPainterPath LogicWire::shape() const
{
    QPainterPath path;
    if (mDirection == WireDirection::HORIZONTAL)
    {
        path.addRect(0, mHeight * -0.5f, mWidth, mHeight);
    }
    else
    {
        path.addRect(mWidth * -0.5f, 0, mWidth, mHeight);
    }
    return path;
}
