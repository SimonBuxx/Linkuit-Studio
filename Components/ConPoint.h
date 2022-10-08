#ifndef CONPOINT_H
#define CONPOINT_H

#include "IBaseComponent.h"
#include "HelperStructures.h"
#include "Components/LogicWireCell.h"
#include "Components/LogicDiodeCell.h"

///
/// \brief The ConPoint class represents a wire connection point
///
class ConPoint : public IBaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for ConPoint
    /// \param pCoreLogic: Pointer to the core logic
    ConPoint(const CoreLogic* pCoreLogic);

    /// \brief Copy constructor for ConPoint
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    ConPoint(const ConPoint& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    ConPoint(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

    /// \brief Clone function for the connection point
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Changes the connection type to the next type
    /// \param pEvent: A mouse press event
    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Selects the wires beyond this ConPoint and moves them
    /// \param pEvent: A mouse move event
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Leaves the mouse moving mode, if it was active
    /// \param pEvent: A mouse release event
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

    /// \brief Returns the connection type of this ConPoint
    /// \return This ConPoints connection type (full connection or diode)
    ConnectionType GetConnectionType(void) const;

    /// \brief Sets the connection type (ConPoint, diode, etc.) to the new type
    /// \param pNewType: The new connection type
    void SetConnectionType(ConnectionType pNewType);

    /// \brief Set the connection type to the next type
    /// \return The previous connection type
    ConnectionType AdvanceConnectionType(void);

    /// \brief Sets the logic cell to the cell of the associated wire group or to a diode logic cell
    /// \param pLogicCell: The logic cell
    void SetLogicCell(const std::shared_ptr<LogicBaseCell>& pLogicCell);

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const override;

    /// \brief Gets the minimum version compatible with this component
    /// \return the minimum version
    virtual SwVersion GetMinVersion(void) const override;

signals:
    /// \brief Emitted when the connection type of this ConPoint changed
    /// \param pConPoint: Pointer to this ConPoint
    /// \param pPreviousType: The previous connection type
    /// \param pCurrentType: The current connection type
    void ConnectionTypeChangedSignal(ConPoint* pConPoint, ConnectionType pPreviousType, ConnectionType pCurrentType);

protected:
    /// \brief Connects this ConPoint to the core logic to notify it when the connection type changes
    /// \param pCoreLogic: Pointer to the core logic
    void ConnectToCoreLogic(const CoreLogic* pCoreLogic);

    /// \brief Paints the connection point
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    bool mWasMoved;
    ConnectionType mConnectionType;

    std::shared_ptr<LogicDiodeCell> mLogicDiodeCell;
};

#endif // CONPOINT_H
