#include "LogicInput.h"
#include "Configuration.h"

LogicInput::LogicInput(const CoreLogic* pCoreLogic):
    BaseComponent(pCoreLogic)
{
    setZValue(components::zvalues::INPUT);

    mWidth = canvas::GRID_SIZE;
    mHeight = canvas::GRID_SIZE;
    mState = LogicState::LOW;
}

LogicInput::LogicInput(const LogicInput& pObj, const CoreLogic* pCoreLogic):
    LogicInput(pCoreLogic)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mState = pObj.mState;
};

BaseComponent* LogicInput::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new LogicInput(*this, pCoreLogic);
}

void LogicInput::ResetZValue()
{
    setZValue(components::zvalues::INPUT);
}

void LogicInput::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

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

    if (levelOfDetail >= components::ROUNDED_CORNERS_MIN_LOD)
    {
        pPainter->drawRoundedRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight, 2, 2);
    }
    else
    {
        pPainter->drawRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
    }
}

QRectF LogicInput::boundingRect() const
{
    return QRectF(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
}

QPainterPath LogicInput::shape() const
{
    QPainterPath path;
    path.addRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
    return path;
}
