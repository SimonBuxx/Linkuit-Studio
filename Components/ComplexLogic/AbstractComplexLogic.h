#ifndef ABSTRACTCOMPLEXLOGIC_H
#define ABSTRACTCOMPLEXLOGIC_H

#include "../IBaseComponent.h"

class AbstractComplexLogic : public IBaseComponent
{
    Q_OBJECT
public:
    /// \brief Constructor for the abstract complex logic component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pLogicCell: Pointer to the associated logic cell
    ///
    ///
    /// \param pDirection: The direction of the component
    AbstractComplexLogic(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell, uint8_t pInputCount, uint8_t pOutputCount, Direction pDirection);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override = 0;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

    /// \brief Defines the shape of this component
    /// \return A rectangular path describing the component shape
    QPainterPath shape(void) const override;

    /// \brief Sets the Z-value to its defined value, to reset it after components have been copied
    void ResetZValue(void) override;

protected:
    /// \brief Paints the abstract complex logic component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    QString mComponentText;

    uint8_t mInputCount;
    uint8_t mOutputCount;
    Direction mDirection;

    std::vector<QString> mInputLabels;
    std::vector<QString> mOutputLabels;

    std::vector<bool> mInputInverted;
    std::vector<bool> mOutputInverted;
};

#endif // ABSTRACTCOMPLEXLOGIC_H
