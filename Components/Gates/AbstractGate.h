#ifndef ABSTRACTGATE_H
#define ABSTRACTGATE_H

#include "../RectComponent.h"

class AbstractGate : public RectComponent
{
    Q_OBJECT
public:
    AbstractGate(uint8_t pInputCount, Direction pDirection);
    AbstractGate(AbstractGate& pObj);
    virtual BaseComponent* CloneBaseComponent() const override = 0;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

protected:
    void PaintSpecifics(QPainter *pPainter, const double pLevelOfDetail) override;

protected:
    QString mComponentText;

    uint8_t mInputCount;
    Direction mDirection;

    bool mOutputInverted = false;
    std::vector<bool> mInputInverted;
};

#endif // ABSTRACTGATE_H
