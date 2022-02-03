#ifndef UNDOCONFIGURETYPE_H
#define UNDOCONFIGURETYPE_H

#include "UndoBaseType.h"

class UndoConfigureType : public UndoBaseType
{
public:
    UndoConfigureType();
    UndoType Type(void) const override;
};

#endif // UNDOCONFIGURETYPE_H
