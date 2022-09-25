#ifndef LOGICCONSTANT_H
#define LOGICCONSTANT_H

#include "../IBaseComponent.h"
#include "HelperStructures.h"
#include "LogicCells/LogicConstantCell.h"

///
/// \brief The LogicConstant class represents a HIGH or LOW constant input
///
class LogicConstant : public IBaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicConstant
    /// \param pCoreLogic: Pointer to the core logic
    /// \param pState: Logic state of this constant
    LogicConstant(const CoreLogic* pCoreLogic, LogicState pState);

    /// \brief Copy constructor for LogicConstant
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    LogicConstant(const LogicConstant& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    LogicConstant(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

    /// \brief Clone function for the logic constant component
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

    /// \brief Getter for this component's constant state
    /// \return the constant logic state of this constant input
    LogicState GetConstantState(void) const;

protected:
    /// \brief Paints the logic constant component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;
};

#endif // LOGICCONSTANT_H
