#ifndef UNDOADDTYPE_H
#define UNDOADDTYPE_H

#include "UndoBaseType.h"
#include "../Components/IBaseComponent.h"

class UndoAddType : public UndoBaseType
{
public:
    UndoAddType(std::vector<IBaseComponent*> &pAddedComponents);
    UndoAddType(std::vector<IBaseComponent*> &pAddedComponents, std::vector<IBaseComponent*> &pDeletedComponents);
    ~UndoAddType(void);
    Undo::Type Type(void) const override;

    const std::vector<IBaseComponent*>& AddedComponents(void) const;
    const std::vector<IBaseComponent*>& DeletedComponents(void) const;

protected:
    std::vector<IBaseComponent*> mAddedComponents;
    std::vector<IBaseComponent*> mDeletedComponents;
};

#endif // UNDOADDTYPE_H
