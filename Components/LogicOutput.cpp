#include "LogicOutput.h"
#include "Configuration.h"

LogicOutput::LogicOutput()
{
    setZValue(components::outputs::Z_VALUE);

    mWidth = canvas::GRID_SIZE;
    mHeight = canvas::GRID_SIZE;
    mState = LogicState::LOW;
}

LogicOutput::LogicOutput(const LogicOutput& pObj)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mState = pObj.mState;
};

BaseComponent* LogicOutput::CloneBaseComponent() const
{
    return new LogicOutput(*this);
}

void LogicOutput::ResetZValue()
{
    setZValue(components::outputs::Z_VALUE);
}

void LogicOutput::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::BORDER_COLOR,
             components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pPainter->setPen(pen);

    if (mState == LogicState::LOW)
    {
        pPainter->setBrush(QBrush(components::FILL_COLOR));
    }
    else if (mState == LogicState::HIGH)
    {
        pPainter->setBrush(QBrush(Qt::white));
    }

    pPainter->drawEllipse(canvas::GRID_SIZE * - 0.5, canvas::GRID_SIZE * - 0.5, mWidth, mHeight);
}

QRectF LogicOutput::boundingRect() const
{
    return QRectF(canvas::GRID_SIZE * - 0.5, canvas::GRID_SIZE * - 0.5, mWidth, mHeight);
}

QPainterPath LogicOutput::shape() const
{
    QPainterPath path;
    path.addRect(canvas::GRID_SIZE * - 0.5, canvas::GRID_SIZE * - 0.5, mWidth, mHeight);
    return path;
}
