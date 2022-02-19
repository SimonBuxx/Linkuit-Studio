#ifndef LOGICWIRE_H
#define LOGICWIRE_H

#include "BaseComponent.h"
#include "HelperStructures.h"

class LogicWire : public BaseComponent
{
    Q_OBJECT
public:
    LogicWire(const CoreLogic* pCoreLogic, WireDirection pDirection, uint32_t pLength);
    LogicWire(const LogicWire& pObj, const CoreLogic* pCoreLogic);

    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    void ResetZValue(void) override;

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

    void SetLength(uint32_t pLength);
    uint32_t GetLength(void) const;

    WireDirection GetDirection(void) const;
    bool StartsOrEndsIn(QPointF pPoint) const;

protected:
    WireDirection mDirection;

    LogicState mState;
};

#endif // LOGICWIRE_H
