#ifndef ABSTRACTGATE_H
#define ABSTRACTGATE_H

#include "../IBaseComponent.h"

///
/// \brief The AbstractGate class is the super class for all logic gates
///
class AbstractGate : public IBaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for the abstract gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pLogicCell: Pointer to the associated logic cell
    /// \param pInputCount: The number of inputs of the gate
    /// \param pDirection: The direction of the gate
    AbstractGate(const CoreLogic* pCoreLogic, const std::shared_ptr<LogicBaseCell>& pLogicCell, uint8_t pInputCount, Direction pDirection);

    /// \brief Clone function for the gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new gate component
    IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override = 0;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    QJsonObject GetJson(void) const override = 0;

    /// \brief Gets the minimum version compatible with this component
    /// \return the minimum version
    SwVersion GetMinVersion(void) const override = 0;

protected:
    /// \brief Paints the abstract gate component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

    /// \brief Draws the gate in- and output pins and inversion circles for a right-facing component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    void DrawGateDetailsRight(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem);

    /// \brief Draws the gate in- and output pins and inversion circles for a down-facing component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    void DrawGateDetailsDown(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem);

    /// \brief Draws the gate in- and output pins and inversion circles for a left-facing component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    void DrawGateDetailsLeft(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem);

    /// \brief Draws the gate in- and output pins and inversion circles for an up-facing component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    void DrawGateDetailsUp(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem);

    /// \brief Creates logic connectors at all in- and output pin points
    void SetLogicConnectors(void);

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

protected:
    QString mComponentText;

    uint8_t mInputCount;
    Direction mDirection;

    uint8_t mInputsSpacing;
    uint8_t mOutputPositionOffset;
};

#endif // ABSTRACTGATE_H
