#ifndef UNDOADDTYPE_H
#define UNDOADDTYPE_H

#include "UndoBaseType.h"
#include "../Components/BaseComponent.h"

class UndoAddType : public UndoBaseType
{
public:
    UndoAddType(std::vector<BaseComponent*> &pAddedComponents);
    UndoAddType(std::vector<BaseComponent*> &pAddedComponents, std::vector<BaseComponent*> &pDeletedComponents);
    UndoType Type(void) const override;

    const std::vector<BaseComponent*>& AddedComponents(void) const;
    const std::vector<BaseComponent*>& DeletedComponents(void) const;

protected:
    std::vector<BaseComponent*> mAddedComponents;
    std::vector<BaseComponent*> mDeletedComponents;
};

#endif // UNDOADDTYPE_H
