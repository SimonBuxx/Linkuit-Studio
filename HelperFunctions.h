#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "Configuration.h"

#include <QCoreApplication>
#include <QGraphicsItem>
#include <QPoint>

static const std::map<ComponentType, ConfiguratorMode> CONFIGURATOR_MODE_MAP{{ComponentType::AND_GATE, ConfiguratorMode::DIRECTION_AND_INPUT_COUNT},
                                                                             {ComponentType::OR_GATE, ConfiguratorMode::DIRECTION_AND_INPUT_COUNT},
                                                                             {ComponentType::XOR_GATE, ConfiguratorMode::DIRECTION_AND_INPUT_COUNT},
                                                                             {ComponentType::NOT_GATE, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::BUFFER_GATE, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::INPUT, ConfiguratorMode::NO_CONFIGURATION},
                                                                             {ComponentType::CONSTANT, ConfiguratorMode::CONSTANT_STATE},
                                                                             {ComponentType::BUTTON, ConfiguratorMode::NO_CONFIGURATION},
                                                                             {ComponentType::CLOCK, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::OUTPUT, ConfiguratorMode::NO_CONFIGURATION},
                                                                             {ComponentType::TEXT_LABEL, ConfiguratorMode::NO_CONFIGURATION},
                                                                             {ComponentType::HALF_ADDER, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::FULL_ADDER, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::RS_FLIPFLOP, ConfiguratorMode::FLIPFLOP_TYPE},
                                                                             {ComponentType::D_FLIPFLOP, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::T_FLIPFLOP, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::JK_FLIPFLOP, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::RS_MS_FLIPFLOP, ConfiguratorMode::DIRECTION_ONLY},
                                                                             {ComponentType::MULTIPLEXER, ConfiguratorMode::MULTIPLEXER_BITS},
                                                                             {ComponentType::DEMULTIPLEXER, ConfiguratorMode::MULTIPLEXER_BITS},
                                                                             {ComponentType::DECODER, ConfiguratorMode::ENCODER_DECODER},
                                                                             {ComponentType::ENCODER, ConfiguratorMode::ENCODER_DECODER},
                                                                             {ComponentType::SHIFTREGISTER, ConfiguratorMode::SHIFTREGISTER_BITS},
                                                                             {ComponentType::COUNTER, ConfiguratorMode::COUNTER_BITS}};

/// \brief Rounds the given point to the nearest grid point
/// \param pPoint: The point to round
/// \return The rounded point
inline QPointF SnapToGrid(QPointF pPoint)
{
    return QPointF(std::floor(pPoint.x() / canvas::GRID_SIZE + 0.5f) * canvas::GRID_SIZE,
                   std::floor(pPoint.y() / canvas::GRID_SIZE + 0.5f) * canvas::GRID_SIZE);
}

/// \brief Calculates the Euclidian distance between pA and pB
/// \param pA: Point A
/// \param pB: Point B
/// \return The distance between point A and point B
inline uint32_t EuclidianDistance(QPointF pA, QPointF pB)
{
    return std::sqrt(std::pow(pA.x() - pB.x(), 2) + std::pow(pA.y() - pB.y(), 2));
}

/// \brief Creates a superscript string for the given number
/// \param pNumber: The number to write as superscript
/// \return A string containing the number as superscript
inline QString BuildSuperscriptString(uint32_t pNumber)
{
    auto rest = pNumber;
    QString result{""};

    if (rest == 0)
    {
        return helpers::SUPERSCRIPTS[0];
    }

    while (rest != 0)
    {
        result = helpers::SUPERSCRIPTS[rest % 10] + result;
        rest /= 10;
    }

    return result;
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

/// \brief Returns the configurator mode for the given component type
/// \param pType: A component type
/// \return A configurator mode
inline ConfiguratorMode GetConfiguratorModeForComponentType(ComponentType pType)
{
    if (CONFIGURATOR_MODE_MAP.find(pType) != CONFIGURATOR_MODE_MAP.end())
    {
        return CONFIGURATOR_MODE_MAP.at(pType);
    }

    return ConfiguratorMode::NO_CONFIGURATION;
}

/// \brief Returns the absolute path to the runtime config JSON file
/// \return The absolute path as a QString
inline QString GetRuntimeConfigAbsolutePath(void)
{
    // QCoreApplication::applicationDirPath() gets the path to the executable instead of the path from where it is executed
    return QCoreApplication::applicationDirPath() + file::runtime_config::RUNTIME_CONFIG_RELATIVE_PATH;
}

#endif // HELPERFUNCTIONS_H
