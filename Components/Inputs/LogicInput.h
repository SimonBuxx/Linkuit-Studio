#ifndef LOGICINPUT_H
#define LOGICINPUT_H

#include "../IBaseComponent.h"
#include "HelperStructures.h"
#include "LogicCells/LogicInputCell.h"

///
/// \brief The LogicInput class represents a logic switch input
///
class LogicInput : public IBaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicInput
    /// \param pCoreLogic: Pointer to the core logic
    LogicInput(const CoreLogic* pCoreLogic);

    /// \brief Copy constructor for LogicInput
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    LogicInput(const LogicInput& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the input component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Triggers the switch input to toggle if in simulation mode
    /// \param pEvent: Pointer to the mouse press event
    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

protected:
    /// \brief Paints the input component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;
};

#endif // LOGICINPUT_H
