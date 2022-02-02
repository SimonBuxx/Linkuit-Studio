#ifndef HELPERSTRUCTURES_H
#define HELPERSTRUCTURES_H

#include "Configuration.h"

enum class Component
{
    NONE = 0,
    AND_GATE,
    OR_GATE,
    XOR_GATE
};

enum class Direction
{
    RIGHT = 0,
    DOWN,
    LEFT,
    UP
};

enum class ControlMode
{
    EDIT = 0,
    ADD_AND_GATE,
    ADD_OR_GATE,
    ADD_XOR_GATE
};

#endif // HELPERSTRUCTURES_H
