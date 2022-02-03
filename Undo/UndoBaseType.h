#ifndef UNDOBASETYPE_H
#define UNDOBASETYPE_H

#include "HelperStructures.h"

class UndoBaseType
{
public:
    UndoBaseType();
    virtual ~UndoBaseType() = default;
    virtual UndoType Type(void) const = 0;
};

#endif // UNDOBASETYPE_H
