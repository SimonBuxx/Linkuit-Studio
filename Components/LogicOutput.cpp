#include "LogicOutput.h"
#include "Configuration.h"

LogicOutput::LogicOutput(const CoreLogic* pCoreLogic):
    BaseComponent(pCoreLogic)
{
    setZValue(components::zvalues::OUTPUT);

    mWidth = canvas::GRID_SIZE;
    mHeight = canvas::GRID_SIZE;
    mState = LogicState::LOW;
}

LogicOutput::LogicOutput(const LogicOutput& pObj, const CoreLogic* pCoreLogic):
    LogicOutput(pCoreLogic)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mState = pObj.mState;
};

BaseComponent* LogicOutput::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new LogicOutput(*this, pCoreLogic);
}

void LogicOutput::ResetZValue()
{
    setZValue(components::zvalues::OUTPUT);
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

    pPainter->drawEllipse(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
}

QRectF LogicOutput::boundingRect() const
{
    return QRectF(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
}

QPainterPath LogicOutput::shape() const
{
    QPainterPath path;
    path.addRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
    return path;
}
