#include "XorGate.h"

XorGate::XorGate(uint8_t pInputCount, Direction pDirection):
    AbstractGate(pInputCount, pDirection)
{
    mComponentText = components::gates::XOR_TEXT;
}
