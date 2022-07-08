#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "Configuration.h"

#include <QGraphicsItem>
#include <QPoint>

static const std::map<ComponentType, ConfiguratorMode> CONFIGURATOR_MODE_MAP{{ComponentType::AND_GATE, ConfiguratorMode::DIRECTION_AND_INPUT_COUNT},
                                                                             {ComponentType::OR_GATE, ConfiguratorMode::DIRECTION_AND_INPUT_COUNT},
                                                                             {ComponentType::XOR_GATE, ConfiguratorMode::DIRECTION_AND_INPUT_COUNT},
                                                                             {ComponentType::NOT_GATE, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::BUFFER_GATE, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::INPUT, ConfiguratorMode::NO_CONFIGURATION},
                                                                             {ComponentType::BUTTON, ConfiguratorMode::NO_CONFIGURATION},
                                                                             {ComponentType::CLOCK, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::OUTPUT, ConfiguratorMode::NO_CONFIGURATION},
                                                                             {ComponentType::TEXT_LABEL, ConfiguratorMode::NO_CONFIGURATION},
                                                                             {ComponentType::HALF_ADDER, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::FULL_ADDER, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::RS_FLIPFLOP, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::D_FLIPFLOP, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::MULTIPLEXER, ConfiguratorMode::MULTIPLEXER_BITS},
                                                                             {ComponentType::DEMULTIPLEXER, ConfiguratorMode::MULTIPLEXER_BITS}};

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
            throw std::logic_error("Logic state invalid");
        }
    }
    return LogicState::LOW;
}

inline ConfiguratorMode GetConfiguratorModeForComponentType(ComponentType pType)
{
    if (CONFIGURATOR_MODE_MAP.find(pType) != CONFIGURATOR_MODE_MAP.end())
    {
        return CONFIGURATOR_MODE_MAP.at(pType);
    }

    return ConfiguratorMode::NO_CONFIGURATION;
}

#endif // HELPERFUNCTIONS_H
