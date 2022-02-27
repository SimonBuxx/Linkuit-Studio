#ifndef LOGICWIRE_H
#define LOGICWIRE_H

#include "BaseComponent.h"
#include "LogicCells/LogicWireCell.h"
#include "HelperStructures.h"

class LogicWire : public BaseComponent
{
    Q_OBJECT
public:
    LogicWire(const CoreLogic* pCoreLogic, WireDirection pDirection, uint32_t pLength);
    LogicWire(const LogicWire& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the wire component
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

    void SetLength(uint32_t pLength);
    uint32_t GetLength(void) const;

    WireDirection GetDirection(void) const;
    bool StartsOrEndsIn(QPointF pPoint) const;

    void SetLogicCell(std::shared_ptr<LogicWireCell>);

protected:
    /// \brief Paints the wire component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    WireDirection mDirection;
};

#endif // LOGICWIRE_H
