#ifndef IBASECOMPONENT_H
#define IBASECOMPONENT_H

#include "LogicBaseCell.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QJsonObject>
#include <QJsonArray>

class CoreLogic;

struct LogicConnector
{
    LogicConnector(ConnectorType pType, QPointF pPos, uint32_t pNum, QPointF pPinCenterOffset)
    {
        type = pType;
        pos = pPos;
        num = pNum;
        pinCenterOffset = pPinCenterOffset;
    }

    ConnectorType type;
    QPointF pos;
    uint32_t num;
    QPointF pinCenterOffset;
};

#warning missing documentation
class IBaseComponent : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    IBaseComponent(const CoreLogic* pCoreLogic, const std::shared_ptr<LogicBaseCell>& pLogicCell);

    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const = 0;

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    virtual void ResetZValue(void) = 0;

    const std::vector<LogicConnector>& GetInConnectors(void) const;
    const std::vector<LogicConnector>& GetOutConnectors(void) const;

    uint32_t GetInConnectorCount(void) const;
    uint32_t GetOutConnectorCount(void) const;

    std::shared_ptr<LogicBaseCell> GetLogicCell(void);

    std::optional<const LogicConnector*> InvertConnectorByPoint(QPointF pPoint);

    QPainterPath shape(void) const override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const = 0;

protected slots:
    void OnLogicStateChanged(void);

signals:
    void SelectedComponentMovedSignal(QPointF pOffset);

protected:
    uint32_t mWidth;
    uint32_t mHeight;

    QPointF mMoveStartPoint;

    bool mSimulationRunning;

    std::vector<LogicConnector> mInConnectors;
    std::vector<LogicConnector> mOutConnectors;
    std::shared_ptr<LogicBaseCell> mLogicCell;

    QPainterPath mShape;
};

#endif // IBASECOMPONENT_H
