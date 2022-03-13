#ifndef LOGICWIRE_H
#define LOGICWIRE_H

#include "IBaseComponent.h"
#include "Components/LogicWireCell.h"
#include "HelperStructures.h"

///
/// \brief The LogicWire class represents one logic wire (horizontal or vertical)
///
class LogicWire : public IBaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicWire
    /// \param pCoreLogic: Pointer to the core logic
    /// \param pDirection: The direction of the wire
    /// \param pLength: The length of the wire in unscaled pixels
    LogicWire(const CoreLogic* pCoreLogic, WireDirection pDirection, uint32_t pLength);

    /// \brief Copy constructor for LogicWire
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    LogicWire(const LogicWire& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the wire component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Defines the shape of this component
    /// \return A rectangular path describing the component shape
    QPainterPath shape(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

    /// \brief Setter for the length (width or height, depending on direction) of this wire
    /// \param pLength: The new length of the wire in unscaled pixels
    void SetLength(uint32_t pLength);

    /// \brief Getter for the length (width or height, depending on direction) of this wire
    /// \return The length of the wire in unscaled pixels
    uint32_t GetLength(void) const;

    /// \brief Getter for the direction of this wire
    /// \return The direction (HORIZONTAL or VERTICAL)
    WireDirection GetDirection(void) const;

    /// \brief Determines whether this wire starts or ends in the given point
    /// \param pPoint: The point to check
    /// \return True, if the wire starts or ends in pPoint
    bool StartsOrEndsIn(QPointF pPoint) const;

    /// \brief Sets the logic wire cell of this wire and connects it to this wire via signals and slots
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
