#ifndef HELPERSTRUCTURES_H
#define HELPERSTRUCTURES_H

enum class ComponentType
{
    NONE = 0,
    AND_GATE,
    OR_GATE,
    XOR_GATE,
    NOT_GATE,
    INPUT,
    BUTTON,
    CLOCK,
    OUTPUT,
    TEXT_LABEL,
    HALF_ADDER,
    FULL_ADDER,
    RS_FLIPFLOP,
    D_FLIPFLOP
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

namespace Undo
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
    CONNECTION_TYPE,
    TEXTLABEL_CONTENT,
    CONNECTOR_INVERSION
};
} // namespace

#endif // HELPERSTRUCTURES_H
