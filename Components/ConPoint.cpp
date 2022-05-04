#include "ConPoint.h"
#include "View/View.h"
#include "Configuration.h"
#include "HelperFunctions.h"

ConPoint::ConPoint(const CoreLogic* pCoreLogic):
    IBaseComponent(pCoreLogic, nullptr),
    mLogicDiodeCell(std::make_shared<LogicDiodeCell>(pCoreLogic))
{
    setZValue(components::zvalues::CONPOINT);

    mWidth = components::conpoints::SIZE;
    mHeight = components::conpoints::SIZE;

    mConnectionType = ConnectionType::FULL;

    mWasMoved = false;

    ConnectToCoreLogic(pCoreLogic);

    // Connectors for diodes
    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 0), 0, QPointF(0, 0)));
    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(0, 0), 0, QPointF(0, 0)));

    mShape.addRect(-components::wires::BOUNDING_RECT_SIZE / 2, -components::wires::BOUNDING_RECT_SIZE / 2,
                 components::wires::BOUNDING_RECT_SIZE, components::wires::BOUNDING_RECT_SIZE);

    QObject::connect(mLogicDiodeCell.get(), &LogicBaseCell::StateChangedSignal, this, &ConPoint::OnLogicStateChanged);
}

ConPoint::ConPoint(const ConPoint& pObj, const CoreLogic* pCoreLogic):
    ConPoint(pCoreLogic)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
    mConnectionType = pObj.mConnectionType;
};

void ConPoint::ConnectToCoreLogic(const CoreLogic* pCoreLogic)
{
    QObject::connect(this, &ConPoint::ConnectionTypeChangedSignal, pCoreLogic, &CoreLogic::OnConnectionTypeChanged);
}

IBaseComponent* ConPoint::CloneBaseComponent(const CoreLogic* pCoreLogic) const
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
    Q_ASSERT(mLogicDiodeCell);
    const auto previous = mConnectionType;
    switch (mConnectionType)
    {
        case ConnectionType::FULL:
        {
            mConnectionType = ConnectionType::DIODE_Y;
            mLogicCell = mLogicDiodeCell;
            break;
        }
        case ConnectionType::DIODE_Y:
        {
            mConnectionType = ConnectionType::DIODE_X;
            mLogicCell = mLogicDiodeCell;
            break;
        }
        case ConnectionType::DIODE_X:
        {
            mConnectionType = ConnectionType::FULL;
            mLogicCell = nullptr; // Will assume the logic cell of its wire group
            break;
        }
        default:
        {
            throw std::logic_error("Connection type invalid");
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
        if ((std::dynamic_pointer_cast<LogicWireCell>(mLogicCell) != nullptr && std::static_pointer_cast<LogicWireCell>(mLogicCell)->GetOutputState() == LogicState::HIGH)
                || (std::dynamic_pointer_cast<LogicDiodeCell>(mLogicCell) != nullptr && std::static_pointer_cast<LogicDiodeCell>(mLogicCell)->GetOutputState() == LogicState::HIGH))
        {
            pPainter->setBrush(components::conpoints::CONPOINTS_HIGH_COLOR);
        }
        else
        {
            pPainter->setBrush(pOption->state & QStyle::State_Selected ? components::SELECTED_COMPONENT_COLOR : components::conpoints::CONPOINTS_LOW_COLOR);
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
                throw std::logic_error("Connection type invalid");
            }
        }
    }
}

void ConPoint::SetLogicCell(std::shared_ptr<LogicBaseCell> pLogicCell)
{
    Q_ASSERT(pLogicCell);
    mLogicCell = pLogicCell;

    QObject::connect(mLogicCell.get(), &LogicBaseCell::StateChangedSignal, this, &ConPoint::OnLogicStateChanged);
}

void ConPoint::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (!mWasMoved && isSelected() && scene()->selectedItems().size() == 1)
    {
        auto prevType = AdvanceConnectionType();
        emit ConnectionTypeChangedSignal(this, prevType, mConnectionType);
    }

    IBaseComponent::mousePressEvent(pEvent);
}

void ConPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (isSelected() && !mWasMoved && scene()->selectedItems().size() == 1)
    {
        // If selected and moved, pull underlying wires with it
        for (auto& item : collidingItems())
        {
            item->setSelected(true);
        }
        mWasMoved = true;
    }

    IBaseComponent::mouseMoveEvent(pEvent);
}

void ConPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    IBaseComponent::mouseReleaseEvent(pEvent);
    mWasMoved = false;
}

QRectF ConPoint::boundingRect() const
{
    return QRectF(-components::wires::BOUNDING_RECT_SIZE / 2, -components::wires::BOUNDING_RECT_SIZE / 2,
                  components::wires::BOUNDING_RECT_SIZE, components::wires::BOUNDING_RECT_SIZE);
}
