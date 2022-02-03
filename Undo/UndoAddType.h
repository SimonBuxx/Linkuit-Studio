#ifndef UNDOADDTYPE_H
#define UNDOADDTYPE_H

#include "UndoBaseType.h"
#include "../Components/BaseComponent.h"

class UndoAddType : public UndoBaseType
{
public:
    UndoAddType(std::vector<BaseComponent*> &pAddedComponents);
    UndoType Type(void) const override;

    const std::vector<BaseComponent*>& Components(void) const;

protected:
    std::vector<BaseComponent*> mComponents;
};

#endif // UNDOADDTYPE_H
