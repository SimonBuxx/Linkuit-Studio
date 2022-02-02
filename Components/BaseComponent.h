#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class BaseComponent : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    BaseComponent(void);
    virtual BaseComponent* CloneBaseComponent() const = 0;
};

#endif // BASECOMPONENT_H
