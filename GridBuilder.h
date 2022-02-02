#ifndef GRIDBUILDER_H
#define GRIDBUILDER_H

#include <QPixmap>
#include <QPainter>

class GridBuilder
{
public:
    GridBuilder();
    static QPixmap DrawGridPattern(int32_t pZoomLevel);
};

#endif // GRIDBUILDER_H
