#include "UndoConfigureType.h"

UndoConfigureType::UndoConfigureType()
{}

UndoType UndoConfigureType::Type() const
{
    return UndoType::CONFIGURE;
}
