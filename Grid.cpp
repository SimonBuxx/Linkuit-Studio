#include "Grid.h"

Grid::Grid(uint32_t pWidth, uint32_t pHeight)
{
    mWidth = pWidth;
    mHeight = pHeight;

    setPos(0, 0);
    setZValue(0);
}

QRectF Grid::boundingRect() const
{
    return QRectF(0, 0, mWidth, mHeight);
}

/*QPainterPath Grid::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, mWidth, mHeight);
    return path;
}*/

void Grid::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget); // suppress a unused var warning
    Q_UNUSED(pOption);

   // const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    QPainterPath path;
    path.addRect(0, 0, mWidth, mHeight);
    QPen pen(Qt::black);
    pPainter->setPen(pen);
    pPainter->fillPath(path, Qt::green);
    pPainter->drawPath(path);
}
