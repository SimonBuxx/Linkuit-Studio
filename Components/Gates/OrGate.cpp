#include "OrGate.h"

OrGate::OrGate(uint8_t pInputCount, Direction pDirection):
    AbstractGate(pInputCount, pDirection)
{
    mComponentText = components::gates::OR_TEXT;
}
