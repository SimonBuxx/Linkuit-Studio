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
    OUTPUT
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

enum class LogicState
{
    LOW = 0,
    HIGH = 1
};

enum class ControlMode
{
    EDIT = 0,
    ADD,
    WIRE
};

enum class UndoType
{
    MOVE = 0,
    ADD,
    DEL,
    CONFIGURE
};

#endif // HELPERSTRUCTURES_H
