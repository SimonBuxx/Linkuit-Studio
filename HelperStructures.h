#ifndef HELPERSTRUCTURES_H
#define HELPERSTRUCTURES_H

#include <QString>

namespace helpers
{
static const QString SUPERSCRIPTS = "⁰¹²³⁴⁵⁶⁷⁸⁹";
} // namespace

enum class ComponentType
{
    NONE = 0,
    AND_GATE,
    OR_GATE,
    XOR_GATE,
    NOT_GATE,
    BUFFER_GATE,
    INPUT,
    BUTTON,
    CLOCK,
    OUTPUT,
    TEXT_LABEL,
    HALF_ADDER,
    FULL_ADDER,
    RS_FLIPFLOP,
    D_FLIPFLOP,
    MULTIPLEXER,
    DEMULTIPLEXER
};

enum class Direction
{
    RIGHT = 0,
    DOWN,
    LEFT,
    UP
};

enum class WireDirection
{
    HORIZONTAL = 0,
    VERTICAL,
    UNSET
};

enum class DiodeDirection
{
    HORIZONTAL = 0,
    VERTICAL
};

enum class LogicState
{
    LOW = 0,
    HIGH = 1
};

enum class ConnectionType
{
    FULL = 0,
    DIODE_X,
    DIODE_Y
};

enum class ConnectorType
{
    IN = 0,
    OUT
};

enum class ControlMode
{
    EDIT = 0,
    ADD,
    WIRE,
    SIMULATION
};

enum class SimulationMode
{
    STOPPED = 0,
    RUNNING
};

namespace undo
{
enum class Type
{
    MOVE = 0,
    ADD,
    DEL,
    CONFIGURE
};

enum class ConfigType
{
    CONNECTION_TYPE = 0,
    TEXTLABEL_CONTENT,
    CONNECTOR_INVERSION
};
} // namespace

#endif // HELPERSTRUCTURES_H
