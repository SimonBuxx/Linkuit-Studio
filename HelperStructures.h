#ifndef HELPERSTRUCTURES_H
#define HELPERSTRUCTURES_H

enum class ComponentType
{
    NONE = 0,
    AND_GATE,
    OR_GATE,
    XOR_GATE,
    NOT_GATE
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
    UNSET // For the preview wires start direction
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
