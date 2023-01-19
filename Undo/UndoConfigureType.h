#ifndef UNDOCONFIGURETYPE_H
#define UNDOCONFIGURETYPE_H

#include "UndoBaseType.h"
#include "Components/ConPoint.h"
#include "Components/TextLabel.h"
#include "HelperStructures.h"

namespace undo
{

/// \brief The abstract ConfigChangedData struct represents all data neccessary to revert the contained configuration change
struct ConfigChangedData
{
    /// \brief Default constructor for this struct
    virtual ~ConfigChangedData() = default;

    /// \brief Getter for the type of this config data
    /// \return The ConfigType of the data contained in this struct
    virtual ConfigType Type(void) const = 0;
};

/// \brief Concrete config data for a connection type change (e.g. change from ConPoint to diode by clicking)
struct ConnectionTypeChangedData : ConfigChangedData
{
    /// \brief Constructor for ConnectionTypeChangedData
    /// \param pConPoint: The ConPoint that has been altered
    /// \param pPreviousType: The previous ConnectionType of this ConPoint
    /// \param pCurrentType: The current ConnectionType of this ConPoint
    ConnectionTypeChangedData(ConPoint* pConPoint, ConnectionType pPreviousType, ConnectionType pCurrentType):
        conPoint(pConPoint),
        previousType(pPreviousType),
        currentType(pCurrentType)
    {}

    /// \brief Getter for the type of this config data
    /// \return The ConfigType of the data contained in this struct
    ConfigType Type(void) const override
    {
        return ConfigType::CONNECTION_TYPE;
    }

    ConPoint* conPoint;
    ConnectionType previousType;
    ConnectionType currentType;
};

/// \brief Concrete config data for a connector inversion change (change from normal to inverted in/output or vice versa)
struct ConnectorInversionChangedData : ConfigChangedData
{
    /// \brief Constructor for ConnectorInversionChangedData
    /// \param pComponent: The component that has been altered
    /// \param pLogicConnector: Data to identify the altered connector pin
    ConnectorInversionChangedData(IBaseComponent* pComponent, const LogicConnector* pLogicConnector):
        component(pComponent),
        logicConnector(pLogicConnector)
    {}

    /// \brief Getter for the type of this config data
    /// \return The ConfigType of the data contained in this struct
    ConfigType Type(void) const override
    {
        return ConfigType::CONNECTOR_INVERSION;
    }

    IBaseComponent* component;
    const LogicConnector* logicConnector;
};

/// \brief Concrete config data for a text label content change
struct TextLabelContentChangedData : ConfigChangedData
{
    /// \brief Constructor for TextLabelContentChangedData
    /// \param pTextLabel: The text label that has been altered
    /// \param pPreviousText: The previous text content of the label
    /// \param pCurrentText: The current text content of the label
    TextLabelContentChangedData(TextLabel* pTextLabel, const QString &pPreviousText, const QString &pCurrentText):
        textLabel(pTextLabel),
        previousText(pPreviousText),
        currentText(pCurrentText)
    {}

    /// \brief Getter for the type of this config data
    /// \return The ConfigType of the data contained in this struct
    ConfigType Type(void) const override
    {
        return ConfigType::TEXTLABEL_CONTENT;
    }

    TextLabel* textLabel;
    QString previousText;
    QString currentText;
};
} // namespace

///
/// \brief The UndoConfigureType class represents an undo action where an object has been altered
///
class UndoConfigureType : public UndoBaseType
{
public:
    /// \brief Constructor for UndoConfigureType
    /// \param pData: The data of the configuration event
    UndoConfigureType(const std::shared_ptr<undo::ConfigChangedData>& pData);

    /// \brief Returns the undo::Type of this class
    /// \return The undo::Type of this class
    undo::Type Type(void) const override;

    /// \brief Returns a pointer to the config data
    /// \return Shared pointer to the config data
    std::shared_ptr<undo::ConfigChangedData> Data(void);

protected:
    std::shared_ptr<undo::ConfigChangedData> mData;
};

#endif // UNDOCONFIGURETYPE_H
