#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

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

inline uint32_t EuclidianDistance(QPointF pA, QPointF pB)
{
    return std::sqrt(std::pow(pA.x() - pB.x(), 2) + std::pow(pA.y() - pB.y(), 2));
}

/// \brief Inverts the given logic state
/// \param pState: The state to invert
/// \return The inverted state
inline LogicState InvertState(LogicState pState)
{
    switch (pState)
    {
        case LogicState::LOW:
        {
            return LogicState::HIGH;
        }
        case LogicState::HIGH:
        {
            return LogicState::LOW;
        }
        default:
        {
            Q_ASSERT(false);
            break;
        }
    }
}

#endif // HELPERFUNCTIONS_H
