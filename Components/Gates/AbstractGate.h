#ifndef ABSTRACTGATE_H
#define ABSTRACTGATE_H

#include "../RectComponent.h"

class AbstractGate : public RectComponent
{
    Q_OBJECT
public:
    AbstractGate(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell, uint8_t pInputCount, Direction pDirection);
    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override = 0;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void ResetZValue(void) override;

protected:
    void PaintSpecifics(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, const double pLevelOfDetail) override;

protected:
    QString mComponentText;

    uint8_t mInputCount;
    Direction mDirection;

    bool mOutputInverted = false;
    std::vector<bool> mInputInverted;
};

#endif // ABSTRACTGATE_H
