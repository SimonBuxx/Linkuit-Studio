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

///
/// \brief The IBaseComponent class represents a generic scene component
///
class IBaseComponent : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    /// \brief Constructor for the abstract IBaseComponent
    /// \param pCoreLogic: Pointer to the core logic
    /// \param pLogicCell: Pointer to the logic cell of this component
    IBaseComponent(const CoreLogic* pCoreLogic, const std::shared_ptr<LogicBaseCell>& pLogicCell);

    /// \brief Returns a new object that is a clone of this object
    /// \param pCoreLogic: Pointer to the core logic
    /// \return Pointer to a new IBaseComponent
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const = 0;

    /// \brief Overloaded mouse press event to control position and bring the component to front
    /// \param pEvent: Pointer to the mouse event
    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Overloaded mouse release event to emit a moved signal and reset the z value of this component
    /// \param pEvent: Pointer to the mouse event
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    virtual void ResetZValue(void) = 0;

    /// \brief Getter for the input connectors of this component
    /// \return Vector of logic connectors
    const std::vector<LogicConnector>& GetInConnectors(void) const;

    /// \brief Getter for the output connectors of this component
    /// \return Vector of logic connectors
    const std::vector<LogicConnector>& GetOutConnectors(void) const;

    /// \brief Getter for the amount of input connectors of this component
    /// \return The amount of input connectors of this component
    uint32_t GetInConnectorCount(void) const;

    /// \brief Getter for the amount of output connectors of this component
    /// \return The amount of output connectors of this component
    uint32_t GetOutConnectorCount(void) const;

    /// \brief Getter for this component's logic cell
    /// \return Smart pointer to the logic cell
    std::shared_ptr<LogicBaseCell> GetLogicCell(void);

    /// \brief Inverts the logic connector at the given positiion
    /// \param pPoint: A point in the scene's coordinates
    /// \return Pointer to the inverted logic connector if it exists
    std::optional<const LogicConnector*> InvertConnectorByPoint(QPointF pPoint);

    /// \brief Getter for this component's shape
    /// \return The shape as a QPainterPath object
    QPainterPath shape(void) const override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const = 0;

protected slots:
    /// \brief Updates when this component's logic state changed
    void OnLogicStateChanged(void);

signals:
    /// \brief Emitted when this component has been moved while selected
    /// \param pOffset: The offset by which the component was moved
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
