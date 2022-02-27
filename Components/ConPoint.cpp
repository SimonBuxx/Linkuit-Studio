#include "ConPoint.h"
#include "View/View.h"
#include "Configuration.h"
#include "HelperFunctions.h"

ConPoint::ConPoint(const CoreLogic* pCoreLogic):
    BaseComponent(pCoreLogic, nullptr),
    mState(LogicState::LOW)
{
    setZValue(components::zvalues::CONPOINT);

    mWidth = components::conpoints::SIZE;
    mHeight = components::conpoints::SIZE;

    mConnectionType = ConnectionType::FULL;

    mWasMoved = false;

    ConnectToCoreLogic(pCoreLogic);
}

ConPoint::ConPoint(const ConPoint& pObj, const CoreLogic* pCoreLogic):
    ConPoint(pCoreLogic)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mState = pObj.mState;
    mConnectionType = pObj.mConnectionType;
};

void ConPoint::ConnectToCoreLogic(const CoreLogic* pCoreLogic)
{
    QObject::connect(this, &ConPoint::ConnectionTypeChangedSignal, pCoreLogic, &CoreLogic::OnConnectionTypeChanged);
}

BaseComponent* ConPoint::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new ConPoint(*this, pCoreLogic);
}

void ConPoint::ResetZValue()
{
    setZValue(components::zvalues::CONPOINT);
}

ConnectionType ConPoint::GetConnectionType() const
{
    return mConnectionType;
}

void ConPoint::SetConnectionType(ConnectionType pNewType)
{
    mConnectionType = pNewType;
    update();
}

ConnectionType ConPoint::AdvanceConnectionType()
{
    const auto previous = mConnectionType;
    switch (mConnectionType)
    {
        case ConnectionType::FULL:
        {
            mConnectionType = ConnectionType::DIODE_Y;
            break;
        }
        case ConnectionType::DIODE_Y:
        {
            mConnectionType = ConnectionType::DIODE_X;
            break;
        }
        case ConnectionType::DIODE_X:
        {
            // CoreLogic will handle deleting if on full crossing
            mConnectionType = ConnectionType::FULL;
            break;
        }
        default:
        {
            Q_ASSERT(false);
            break;
        }
    }
    return previous;
}

void ConPoint::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    if (levelOfDetail >= components::conpoints::MIN_VISIBLE_LOD)
    {
        if (mState == LogicState::LOW)
        {
            pPainter->setBrush(pOption->state & QStyle::State_Selected ? components::SELECTED_COMPONENT_COLOR : components::conpoints::CONPOINTS_LOW_COLOR);
        }
        else if (mState == LogicState::HIGH)
        {
            pPainter->setBrush(Qt::white);
        }
        pPainter->setPen(Qt::NoPen);

        switch (mConnectionType)
        {
            case ConnectionType::FULL:
            {
                pPainter->drawRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
                break;
            }
            case ConnectionType::DIODE_X:
            {
                pPainter->drawConvexPolygon({QPointF(components::BORDER_WIDTH * -0.5f, components::diodes::SIZE * -0.5f),
                                             QPointF(components::diodes::SIZE - 5, 0),
                                             QPointF(components::BORDER_WIDTH * -0.5f, components::diodes::SIZE * 0.5f)});
                break;
            }
            case ConnectionType::DIODE_Y:
            {
                pPainter->drawConvexPolygon({QPointF(components::diodes::SIZE * -0.5f, components::BORDER_WIDTH * -0.5f),
                                            QPointF(0, components::diodes::SIZE - 5),
                                            QPointF(components::diodes::SIZE * 0.5f, components::BORDER_WIDTH * -0.5f)});
                break;
            }
            default:
            {
                Q_ASSERT(false);
                break;
            }
        }
    }
}

void ConPoint::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (!mWasMoved && isSelected() && scene()->selectedItems().size() == 1)
    {
        auto prevType = AdvanceConnectionType();
        emit ConnectionTypeChangedSignal(this, prevType, mConnectionType);
    }

    BaseComponent::mousePressEvent(pEvent);
}

void ConPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (scene()->selectedItems().size() == 1)
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
    }
    else
    {
        setSelected(false);
    }

    BaseComponent::mouseMoveEvent(pEvent);
}

void ConPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    BaseComponent::mouseReleaseEvent(pEvent);
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
