#ifndef ABSTRACTGATE_H
#define ABSTRACTGATE_H

#include "../BaseComponent.h"

class AbstractGate : public BaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for the abstract gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pLogicCell: Pointer to the associated logic cell
    /// \param pInputCount: The number of inputs of the gate
    /// \param pDirection: The direction of the gate
    AbstractGate(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell, uint8_t pInputCount, Direction pDirection);

    /// \brief Clone function for the gate
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new gate component
    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override = 0;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Defines the shape of this component
    /// \return A rectangular path describing the component shape
    QPainterPath shape(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

protected:
    /// \brief Paints the abstract gate component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    QString mComponentText;

    uint8_t mInputCount;
    Direction mDirection;

    bool mOutputInverted = false;
    std::vector<bool> mInputInverted;
};

#endif // ABSTRACTGATE_H
