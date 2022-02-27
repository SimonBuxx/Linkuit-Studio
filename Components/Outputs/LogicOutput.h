#ifndef LOGICOUTPUT_H
#define LOGICOUTPUT_H

#include "../BaseComponent.h"
#include "HelperStructures.h"
#include "LogicCells/LogicOutputCell.h"

class LogicOutput : public BaseComponent
{
    Q_OBJECT
public:
    LogicOutput(const CoreLogic* pCoreLogic);
    LogicOutput(const LogicOutput& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the output component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Defines the shape of this component
    /// \return A rectangular path describing the component shape
    QPainterPath shape(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

protected:
    /// \brief Paints the output component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;
};

#endif // LOGICOUTPUT_H
