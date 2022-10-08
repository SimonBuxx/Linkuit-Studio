#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include "AbstractComplexLogic.h"

///
/// \brief The ShiftRegister class represents a logic shift register
///
class ShiftRegister : public AbstractComplexLogic
{
    Q_OBJECT
public:
    /// \brief Constructor for ShiftRegister
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pDirection: The direction of the component
    /// \param pBitWidth: The size of the register in bits
    ShiftRegister(const CoreLogic* pCoreLogic, Direction pDirection, uint8_t pBitWidth);

    /// \brief Copy constructor for ShiftRegister
    /// \param pObj: The object to be copied
    /// \param pCoreLogic: Pointer to the core logic
    ShiftRegister(const ShiftRegister& pObj, const CoreLogic* pCoreLogic);

    /// \brief Constructor for loading from JSON
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \param pJson: The JSON object to load the component's data from
    ShiftRegister(const CoreLogic* pCoreLogic, const QJsonObject& pJson);

    /// \brief Clone function for the component
    /// \param pCoreLogic: Pointer to the core logic, used to connect the component's signals and slots
    /// \return A pointer to the new component
    virtual IBaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    /// \brief Saves the dats of this component to the given JSON object
    /// \return The JSON object with the component data
    virtual QJsonObject GetJson(void) const override;

    /// \brief Gets the minimum version compatible with this component
    /// \return the minimum version
    virtual SwVersion GetMinVersion(void) const override;

    /// \brief Defines the bounding rect of this component
    /// \return A rectangle describing the bounding rect
    QRectF boundingRect(void) const override;

protected:
    /// \brief Paints the shift register
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    /// \param pWidget: Unused, the widget that is been painted on
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

    /// \brief Draws the component in- and output pins and inversion circles for a right-facing component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    void DrawComponentDetailsRight(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem);

    /// \brief Draws the component in- and output pins and inversion circles for a down-facing component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    void DrawComponentDetailsDown(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem);

    /// \brief Draws the component in- and output pins and inversion circles for a left-facing component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    void DrawComponentDetailsLeft(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem);

    /// \brief Draws the component in- and output pins and inversion circles for an up-facing component
    /// \param pPainter: The painter to use
    /// \param pItem: Contains drawing parameters
    void DrawComponentDetailsUp(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem);

    /// \brief Creates logic connectors at all in- and output pin points
    void SetLogicConnectors(void) override;

protected:
    uint8_t mBitWidth;
};

#endif // SHIFTREGISTER_H
