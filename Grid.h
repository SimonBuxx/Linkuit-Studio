#ifndef GRID_H
#define GRID_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class Grid : public QGraphicsItem
{
public:
    Grid(uint32_t pWidth, uint32_t pHeight);

    QRectF boundingRect(void) const override;
    // QPainterPath shape(void) const override;

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    uint32_t mWidth;
    uint32_t mHeight;
};

#endif // GRID_H
