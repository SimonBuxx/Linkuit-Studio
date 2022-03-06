#ifndef UNDODELETETYPE_H
#define UNDODELETETYPE_H

#include "UndoBaseType.h"
#include "../Components/IBaseComponent.h"

class UndoDeleteType : public UndoBaseType
{
public:
    UndoDeleteType(std::vector<IBaseComponent*> &pDeletedComponents);
    ~UndoDeleteType(void);
    Undo::Type Type(void) const override;

    const std::vector<IBaseComponent*>& Components(void) const;

protected:
    std::vector<IBaseComponent*> mComponents;
};

#endif // UNDODELETETYPE_H
