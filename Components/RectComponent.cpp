#include "RectComponent.h"

RectComponent::RectComponent(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell):
    BaseComponent(pCoreLogic, pLogicCell)
{}

void RectComponent::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::BORDER_COLOR,
             components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pPainter->setPen(pen);
    pPainter->setBrush(QBrush(components::FILL_COLOR));

    if (levelOfDetail >= components::ROUNDED_CORNERS_MIN_LOD)
    {
        pPainter->drawRoundedRect(0, 0, mWidth, mHeight, 2, 2);
    }
    else
    {
        pPainter->drawRect(0, 0, mWidth, mHeight);
    }
    PaintSpecifics(pPainter, pOption, levelOfDetail);
}

QRectF RectComponent::boundingRect() const
{
    return QRectF(0, 0, mWidth, mHeight);
}

QPainterPath RectComponent::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, mWidth, mHeight);
    return path;
}
