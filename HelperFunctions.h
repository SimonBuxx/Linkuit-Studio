#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "Configuration.h"

#include <QPoint>

inline QPointF SnapToGrid(QPointF pPoint)
{
    return QPointF(round(pPoint.x() / canvas::GRID_SIZE) * canvas::GRID_SIZE,
                   round(pPoint.y() / canvas::GRID_SIZE) * canvas::GRID_SIZE);
}

#endif // HELPERFUNCTIONS_H
