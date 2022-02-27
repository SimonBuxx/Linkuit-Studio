#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include "LogicBaseCell.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class CoreLogic;

struct LogicConnector
{
    LogicConnector(ConnectorType pType, QPointF pPos)
    {
        type = pType;
        pos = pPos;
    }

    ConnectorType type;
    QPointF pos;
};

class BaseComponent : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    BaseComponent(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell);

    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const = 0;

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    virtual void ResetZValue(void) = 0;

    std::vector<LogicConnector>& GetInConnectors(void);
    std::vector<LogicConnector>& GetOutConnectors(void);

    uint32_t GetInConnectorCount(void) const;
    uint32_t GetOutConnectorCount(void) const;

    std::shared_ptr<LogicBaseCell> GetLogicCell(void);

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
};

#endif // BASECOMPONENT_H
