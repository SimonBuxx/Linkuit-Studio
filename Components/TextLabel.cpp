#include "TextLabel.h"
#include "Configuration.h"

TextLabel::TextLabel(const CoreLogic* pCoreLogic):
    BaseComponent(pCoreLogic, nullptr)
{
    setZValue(components::zvalues::TEXT_LABEL);

    mHeight = canvas::GRID_SIZE * 0.8f;
    QFontMetrics metrics(components::text_label::FONT);
    mWidth = metrics.boundingRect(mText).width() + canvas::GRID_SIZE / 4;
}

TextLabel::TextLabel(const TextLabel& pObj, const CoreLogic* pCoreLogic):
    TextLabel(pCoreLogic)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
};

BaseComponent* TextLabel::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new TextLabel(*this, pCoreLogic);
}

void TextLabel::ResetZValue()
{
    setZValue(components::zvalues::TEXT_LABEL);
}

void TextLabel::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);
    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    if (levelOfDetail >= components::DESCRIPTION_TEXT_MIN_LOD)
    {
        QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(Qt::NoBrush);

        pPainter->drawLine(0, mHeight * -0.5f, 0, mHeight * 0.5f);

        pPainter->setPen(components::text_label::FONT_COLOR);
        pPainter->setFont(components::text_label::FONT);
        pPainter->drawText(QRect(canvas::GRID_SIZE / 4, mHeight * -0.5f, mWidth - canvas::GRID_SIZE / 4, mHeight), mText, Qt::AlignLeft | Qt::AlignVCenter);
    }
}

QRectF TextLabel::boundingRect() const
{
    return QRectF(0, mHeight * -0.5f, mWidth, mHeight);
}

QPainterPath TextLabel::shape() const
{
    QPainterPath path;
    path.addRect(0, mHeight * -0.5f, mWidth, mHeight);
    return path;
}
