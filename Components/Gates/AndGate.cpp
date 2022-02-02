#include "AndGate.h"

AndGate::AndGate(uint8_t pInputCount, Direction pDirection):
    AbstractGate(pInputCount, pDirection)
{
    mComponentText = components::gates::AND_TEXT;
}
