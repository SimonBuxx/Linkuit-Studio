#ifndef UNDODELETETYPE_H
#define UNDODELETETYPE_H

#include "UndoBaseType.h"
#include "../Components/BaseComponent.h"

class UndoDeleteType : public UndoBaseType
{
public:
    UndoDeleteType(std::vector<BaseComponent*> &pDeletedComponents);
    ~UndoDeleteType(void);
    UndoType Type(void) const override;

    const std::vector<BaseComponent*>& Components(void) const;

protected:
    std::vector<BaseComponent*> mComponents;
};

#endif // UNDODELETETYPE_H
