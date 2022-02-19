#ifndef UNDOCONFIGURETYPE_H
#define UNDOCONFIGURETYPE_H

#include "UndoBaseType.h"
#include "Components/ConPoint.h"
#include "HelperStructures.h"

namespace Undo
{
struct ConfigChangedData
{
    virtual ~ConfigChangedData() = default;
    virtual ConfigType Type(void) const = 0;
};

struct ConnectionTypeChangedData : ConfigChangedData
{
    ConnectionTypeChangedData(ConPoint* pConPoint, ConnectionType pPreviousType, ConnectionType pCurrentType):
        conPoint(pConPoint),
        previousType(pPreviousType),
        currentType(pCurrentType)
    {}

    ConfigType Type(void) const override
    {
        return ConfigType::CONNECTION_TYPE;
    }

    ConPoint* conPoint;
    ConnectionType previousType;
    ConnectionType currentType;
};
}

class UndoConfigureType : public UndoBaseType
{
public:
    /// \brief Constructor for UndoConfigureType
    /// \param pData: The data of the configuration event
    UndoConfigureType(std::shared_ptr<Undo::ConfigChangedData> pData);

    /// \brief Returns the Undo::Type of this class
    /// \return The Undo::Type of this class
    Undo::Type Type(void) const override;

    /// \brief Returns a pointer to the config data
    /// \return Shared pointer to the config data
    std::shared_ptr<Undo::ConfigChangedData> Data(void);

protected:
    std::shared_ptr<Undo::ConfigChangedData> mData;
};

#endif // UNDOCONFIGURETYPE_H
