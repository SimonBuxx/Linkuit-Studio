#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "Components/ConPoint.h"
#include "Configuration.h"

#include <QGraphicsItem>
#include <QPoint>

/// \brief Rounds the given point to the nearest grid point
/// \param pPoint: The point to round
/// \return The rounded point
inline QPointF SnapToGrid(QPointF pPoint)
{
    return QPointF(std::floor(pPoint.x() / canvas::GRID_SIZE + 0.5f) * canvas::GRID_SIZE,
                   std::floor(pPoint.y() / canvas::GRID_SIZE + 0.5f) * canvas::GRID_SIZE);
}

#endif // HELPERFUNCTIONS_H
