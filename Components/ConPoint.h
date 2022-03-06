#ifndef CONPOINT_H
#define CONPOINT_H

#include "IBaseComponent.h"
#include "HelperStructures.h"
#include "Components/LogicWireCell.h"

class ConPoint : public IBaseComponent
{
    Q_OBJECT
public:
    ConPoint(const CoreLogic* pCoreLogic);
    ConPoint(const ConPoint& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the connection point
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Defines the shape of this component
    /// \return A rectangular path describing the component shape
    QPainterPath shape(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

    /// \brief Returns the connection type of this ConPoint
    /// \return This ConPoints connection type (full connection or diode)
    ConnectionType GetConnectionType(void) const;

    void SetConnectionType(ConnectionType pNewType);

    /// \brief Set the connection type to the next type
    /// \return The previous connection type
    ConnectionType AdvanceConnectionType(void);

    void SetLogicCell(std::shared_ptr<LogicWireCell> pLogicCell);

signals:
    void ConnectionTypeChangedSignal(ConPoint* pConPoint, ConnectionType pPreviousType, ConnectionType pCurrentType);

protected:
    void ConnectToCoreLogic(const CoreLogic* pCoreLogic);

    /// \brief Paints the connection point
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    bool mWasMoved;
    ConnectionType mConnectionType;
};

#endif // CONPOINT_H
