#ifndef HELPERSTRUCTURES_H
#define HELPERSTRUCTURES_H

#include <QString>

namespace helpers
{
static const QString SUPERSCRIPTS = "⁰¹²³⁴⁵⁶⁷⁸⁹";
} // namespace

struct SwVersion
{
    SwVersion(uint8_t pMajor, uint8_t pMinor, uint8_t pPatch)
    {
        major = pMajor;
        minor = pMinor;
        patch = pPatch;
    };

    uint8_t major;
    uint8_t minor;
    uint8_t patch;
};

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
        JK_FLIPFLOP,
        DECODER,
        ENCODER,
        SHIFTREGISTER,
        CONSTANT,
        COUNTER,
        RS_MS_FLIPFLOP,
        RS_CLOCKED_FLIPFLOP,
        D_MS_FLIPFLOP // only append!
    };
} // namespace file

enum class ComponentType
{
    NONE = 0,
    AND_GATE,
    OR_GATE,
    XOR_GATE,
    NOT_GATE,
    BUFFER_GATE,
    INPUT,
    CONSTANT,
    BUTTON,
    CLOCK,
    OUTPUT,
    TEXT_LABEL,
    HALF_ADDER,
    FULL_ADDER,
    RS_FLIPFLOP,
    D_FLIPFLOP,
    D_MS_FLIPFLOP,
    MULTIPLEXER,
    DEMULTIPLEXER,
    T_FLIPFLOP,
    JK_FLIPFLOP,
    RS_MS_FLIPFLOP,
    RS_CLOCKED_FLIPFLOP,
    DECODER,
    ENCODER,
    SHIFTREGISTER,
    COUNTER
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

enum class FlipFlopStyle
{
    LATCH = 0,
    CLOCKED,
    MASTER_SLAVE
};

enum class ConfiguratorMode
{
    NO_CONFIGURATION = 0,
    DIRECTION_AND_INPUT_COUNT,
    DIRECTION_ONLY,
    MULTIPLEXER_BITS,
    ENCODER_DECODER,
    SHIFTREGISTER_BITS,
    CONSTANT_STATE,
    COUNTER_BITS,
    RS_FLIPFLOP_TYPE,
    MASTER_SLAVE
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
