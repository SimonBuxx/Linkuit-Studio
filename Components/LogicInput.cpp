#include "LogicInput.h"
#include "Configuration.h"

LogicInput::LogicInput()
{
    setZValue(components::inputs::Z_VALUE);

    mWidth = canvas::GRID_SIZE;
    mHeight = canvas::GRID_SIZE;
    mState = LogicState::LOW;
}

LogicInput::LogicInput(const LogicInput& pObj)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mState =pObj.mState;
};

BaseComponent* LogicInput::CloneBaseComponent() const
{
    return new LogicInput(*this);
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
        pPainter->drawRoundedRect(canvas::GRID_SIZE * - 0.5, canvas::GRID_SIZE * - 0.5, mWidth, mHeight, 2, 2);
    }
    else
    {
        pPainter->drawRect(canvas::GRID_SIZE * - 0.5, canvas::GRID_SIZE * - 0.5, mWidth, mHeight);
    }
}

QRectF LogicInput::boundingRect() const
{
    return QRectF(canvas::GRID_SIZE * - 0.5, canvas::GRID_SIZE * - 0.5, mWidth, mHeight);
}

QPainterPath LogicInput::shape() const
{
    QPainterPath path;
    path.addRect(canvas::GRID_SIZE * - 0.5, canvas::GRID_SIZE * - 0.5, mWidth, mHeight);
    return path;
}
