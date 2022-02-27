#ifndef LOGICINPUT_H
#define LOGICINPUT_H

#include "../BaseComponent.h"
#include "HelperStructures.h"
#include "LogicCells/LogicInputCell.h"

class LogicInput : public BaseComponent
{
    Q_OBJECT
public:
    LogicInput(const CoreLogic* pCoreLogic);
    LogicInput(const LogicInput& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the input component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Defines the shape of this component
    /// \return A rectangular path describing the component shape
    QPainterPath shape(void) const override;

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
