#ifndef CONPOINT_H
#define CONPOINT_H

#include "BaseComponent.h"
#include "HelperStructures.h"

class ConPoint : public BaseComponent
{
    Q_OBJECT
public:
    ConPoint(const CoreLogic* pCoreLogic);
    ConPoint(const ConPoint& pObj, const CoreLogic* pCoreLogic);

    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void ResetZValue(void) override;

    /// \brief Returns the connection type of this ConPoint
    /// \return This ConPoints connection type (full connection or diode)
    ConnectionType GetConnectionType(void) const;

    void SetConnectionType(ConnectionType pNewType);

    /// \brief Set the connection type to the next type
    /// \return The previous connection type
    ConnectionType AdvanceConnectionType(void);

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

signals:
    void ConnectionTypeChangedSignal(ConPoint* pConPoint, ConnectionType pPreviousType, ConnectionType pCurrentType);

protected:
    void ConnectToCoreLogic(const CoreLogic* pCoreLogic);

protected:
    LogicState mState;

    bool mWasMoved;
    ConnectionType mConnectionType;
};

#endif // CONPOINT_H
