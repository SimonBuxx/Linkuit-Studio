#ifndef LOGICOUTPUT_H
#define LOGICOUTPUT_H

#include "../IBaseComponent.h"
#include "HelperStructures.h"
#include "LogicCells/LogicOutputCell.h"

///
/// \brief The LogicOutput class represents a logic output
///
class LogicOutput : public IBaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicOutput
    /// \param pCoreLogic: Pointer to the core logic
    LogicOutput(const CoreLogic* pCoreLogic);

    /// \brief Copy constructor for LogicOutput
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    LogicOutput(const LogicOutput& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    LogicOutput(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

    /// \brief Clone function for the output component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const override;

protected:
    /// \brief Paints the output component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;
};

#endif // LOGICOUTPUT_H
