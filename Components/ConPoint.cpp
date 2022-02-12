#include "ConPoint.h"
#include "View/View.h"
#include "Configuration.h"
#include "HelperFunctions.h"

#include <QGraphicsScene>

ConPoint::ConPoint():
    mState(LogicState::LOW)
{
    setZValue(components::zvalues::CONPOINT);

    mWidth = components::conpoints::SIZE;
    mHeight = components::conpoints::SIZE;

    mWasMoved = false;
    mWasSelectedBefore = false;
}

ConPoint::ConPoint(const ConPoint& pObj)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mState = pObj.mState;
};

BaseComponent* ConPoint::CloneBaseComponent() const
{
    return new ConPoint(*this);
}

void ConPoint::ResetZValue()
{
    setZValue(components::zvalues::CONPOINT);
}

void ConPoint::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    if (levelOfDetail >= components::conpoints::MIN_VISIBLE_LOD)
    {
        if (mState == LogicState::LOW)
        {
            pPainter->setBrush(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::BORDER_COLOR);
        }
        else if (mState == LogicState::HIGH)
        {
            pPainter->setBrush(Qt::white);
        }
        pPainter->setPen(Qt::NoPen);

        pPainter->drawRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
    }
}

void ConPoint::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (isSelected())
    {
        // ConPoint was selected before, change to diode on release
        mWasSelectedBefore = true;
    }

    BaseComponent::mousePressEvent(pEvent);
}

void ConPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
    // If selected and moved, pull underlying wires with it
    if (isSelected() && !mWasMoved)
    {
        for (auto& item : collidingItems())
        {
            item->setSelected(true);
        }
    }

    mWasMoved = true;
    BaseComponent::mouseMoveEvent(pEvent);
}

void ConPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (!mWasMoved && mWasSelectedBefore)
    {
        qDebug() << "change to diode";
    }

    BaseComponent::mouseReleaseEvent(pEvent);

    mWasSelectedBefore = false;
    mWasMoved = false;
}


QRectF ConPoint::boundingRect() const
{
    return QRectF(-components::wires::BOUNDING_RECT_SIZE / 2, -components::wires::BOUNDING_RECT_SIZE / 2,
                  components::wires::BOUNDING_RECT_SIZE, components::wires::BOUNDING_RECT_SIZE);
}

QPainterPath ConPoint::shape() const
{
    QPainterPath path;
    path.addRect(-components::wires::BOUNDING_RECT_SIZE / 2, -components::wires::BOUNDING_RECT_SIZE / 2,
                 components::wires::BOUNDING_RECT_SIZE, components::wires::BOUNDING_RECT_SIZE);
    return path;
}
