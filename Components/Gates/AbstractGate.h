#ifndef ABSTRACTGATE_H
#define ABSTRACTGATE_H

#include "../RectComponent.h"

class AbstractGate : public RectComponent
{
public:
    AbstractGate(uint8_t pInputCount, Direction pDirection);

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

protected:
    void PaintSpecifics(QPainter *pPainter, const double pLevelOfDetail) override;

protected:
    QString mComponentText;

    uint8_t mInputCount;
    Direction mDirection;
};

#endif // ABSTRACTGATE_H
