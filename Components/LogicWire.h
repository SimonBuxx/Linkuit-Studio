#ifndef LOGICWIRE_H
#define LOGICWIRE_H

#include "BaseComponent.h"
#include "HelperStructures.h"
#include "Configuration.h"

class LogicWire : public BaseComponent
{
public:
    LogicWire(WireDirection pDirection, uint32_t pLength);
    LogicWire(const LogicWire& pObj);

    virtual BaseComponent* CloneBaseComponent() const override;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

    void SetLength(uint32_t pLength);
    uint32_t GetLength(void) const;

    WireDirection GetDirection(void) const;

protected:
    WireDirection mDirection;

    LogicState mState;
};

#endif // LOGICWIRE_H
