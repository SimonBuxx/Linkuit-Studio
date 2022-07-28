#ifndef HELPERSTRUCTURES_H
#define HELPERSTRUCTURES_H

#include <QString>

namespace helpers
{
static const QString SUPERSCRIPTS = "⁰¹²³⁴⁵⁶⁷⁸⁹";
} // namespace

namespace file
{
    enum class SaveFormat
    {
        JSON = 0,
        BINARY
    };

    enum ComponentId
    {
        WIRE = 0,
        CONPOINT,
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
        DEMULTIPLEXER,
        T_FLIPFLOP,
        JK_FLIPFLOP // only append!
    };
}

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
    DEMULTIPLEXER,
    T_FLIPFLOP,
    JK_FLIPFLOP
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
    SIMULATION,
    COPY // Active mode after pasting, left after accepting or aborting the pasting process
};

enum class SimulationMode
{
    STOPPED = 0,
    RUNNING
};

enum class ClockMode
{
    TOGGLE = 0,
    PULSE
};

enum class ConfiguratorMode
{
    NO_CONFIGURATION,
    DIRECTION_AND_INPUT_COUNT,
    DIRECTION_ONLY,
    OUTPUT_COLOR,
    MULTIPLEXER_BITS
};

namespace undo
{
enum class Type
{
    MOVE = 0,
    ADD,
    DEL,
    CONFIGURE,
    COPY
};

enum class ConfigType
{
    CONNECTION_TYPE = 0,
    TEXTLABEL_CONTENT,
    CONNECTOR_INVERSION
};
} // namespace

#endif // HELPERSTRUCTURES_H
