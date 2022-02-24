#ifndef RECTCOMPONENT_H
#define RECTCOMPONENT_H

#include "BaseComponent.h"
#include "Configuration.h"

/// \brief Base class for all rectangular components
class RectComponent : public BaseComponent
{
    Q_OBJECT
public:
    RectComponent(const CoreLogic* pCoreLogic, std::shared_ptr<LogicBaseCell> pLogicCell);

    virtual BaseComponent* CloneBaseComponent(const CoreLogic* pCoreLogic) const override = 0;

    QRectF boundingRect(void) const override;
    QPainterPath shape(void) const override;

    virtual void ResetZValue(void) override = 0;

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pItem, QWidget *pWidget) override;

protected:
    virtual void PaintSpecifics(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, const double pLevelOfDetail) = 0;
};

#endif // RECTCOMPONENT_H
