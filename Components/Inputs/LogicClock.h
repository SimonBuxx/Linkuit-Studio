#ifndef LOGICCLOCK_H
#define LOGICCLOCK_H

#include "../IBaseComponent.h"
#include "HelperStructures.h"
#include "LogicCells/LogicClockCell.h"

///
/// \brief The LogicClock class represents a logic clock input
///
class LogicClock : public IBaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicClock
    /// \param pCoreLogic: Pointer to the core logic
    /// \param pDirection: The direction of the clock
    LogicClock(const CoreLogic* pCoreLogic, Direction pDirection);

    /// \brief Copy constructor for LogicClock
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    LogicClock(const LogicClock& pObj, const CoreLogic* pCoreLogic);

    /// \brief Clone function for the clock component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

    /// \brief Emits a signal to show the clock configurator if this is the only selected clock
    /// \param pEvent: The mouse event
    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;

signals:
    /// \brief Emitted when the clock configurator should be displayed with the given settings
    /// \param pMode: The clock mode
    /// \param pToggle: The toggle speed
    /// \param pPulse: The pulse duration
    void ShowClockConfiguratorSignal(ClockMode pMode, int32_t pToggle, int32_t pPulse);

protected:
    /// \brief Paints the clock component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

    /// \brief Draws the clock in- and output pins and inversion circles for a right-facing component
    /// \param pPainter: The painter to use
    /// \param pOption: Contains drawing parameters
    void DrawClockDetailsRight(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption);

    /// \brief Draws the clock in- and output pins and inversion circles for a down-facing component
    /// \param pPainter: The painter to use
    /// \param pOption: Contains drawing parameters
    void DrawClockDetailsDown(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption);

    /// \brief Draws the clock in- and output pins and inversion circles for a left-facing component
    /// \param pPainter: The painter to use
    /// \param pOption: Contains drawing parameters
    void DrawClockDetailsLeft(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption);

    /// \brief Draws the clock in- and output pins and inversion circles for an up-facing component
    /// \param pPainter: The painter to use
    /// \param pOption: Contains drawing parameters
    void DrawClockDetailsUp(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption);

    /// \brief Sets the current pen to the pen to use for connector pins
    /// \param pPainter: The painter to use
    /// \param pState: The logic state of the pin
    /// \param pSelected: Whether the component is selected or not
    void SetConnectorPen(QPainter *pPainter, LogicState pState, bool pSelected);

    /// \brief Sets the current pen and brush to the pen to use for inversion circles
    /// \param pPainter: The painter to use
    /// \param pState: The logic state of the inversion circle
    /// \param pSelected: Whether the component is selected or not
    void SetInversionPen(QPainter *pPainter, LogicState pState, bool pSelected);

    /// \brief Creates logic connectors at all in- and output pin points
    void SetLogicConnectors(void);

protected:
    Direction mDirection;

    QPainterPath mSquareWave;
};

#endif // LOGICCLOCK_H
