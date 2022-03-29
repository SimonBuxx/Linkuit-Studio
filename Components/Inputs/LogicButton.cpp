#include "LogicButton.h"
#include "CoreLogic.h"
#include "Configuration.h"

LogicButton::LogicButton(const CoreLogic* pCoreLogic):
    IBaseComponent(pCoreLogic, std::make_shared<LogicButtonCell>())
{
    setZValue(components::zvalues::INPUT);

    mWidth = canvas::GRID_SIZE * 0.8f;
    mHeight = canvas::GRID_SIZE * 0.8f;

    mShape.addRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);

    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(0, 0), 0, QPointF(0, 0))); // Place connector in the middle of the component

    QObject::connect(pCoreLogic, &CoreLogic::SimulationStartSignal, this, [&]()
    {
        setCursor(Qt::PointingHandCursor);
    });
}

LogicButton::LogicButton(const LogicButton& pObj, const CoreLogic* pCoreLogic):
    LogicButton(pCoreLogic)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
};

IBaseComponent* LogicButton::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new LogicButton(*this, pCoreLogic);
}

void LogicButton::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (mSimulationRunning)
    {
        std::static_pointer_cast<LogicButtonCell>(mLogicCell)->ButtonClick();
    }
    IBaseComponent::mousePressEvent(pEvent);
}

void LogicButton::ResetZValue()
{
    setZValue(components::zvalues::INPUT);
}

void LogicButton::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    if (std::static_pointer_cast<LogicButtonCell>(mLogicCell)->GetOutputState() == LogicState::LOW)
    {
        QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR,
                 components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::FILL_COLOR));
    }
    else if (std::static_pointer_cast<LogicButtonCell>(mLogicCell)->GetOutputState() == LogicState::HIGH)
    {
        QPen pen(components::HIGH_COLOR, components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::HIGH_COLOR));
    }

    pPainter->drawRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);

    if (levelOfDetail >= components::COMPONENT_DETAILS_MIN_LOD)
    {
        if (std::static_pointer_cast<LogicButtonCell>(mLogicCell)->GetOutputState() == LogicState::LOW)
        {
            QPen pen(components::wires::WIRE_LOW_COLOR, components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

            pPainter->setPen(pen);
            pPainter->setBrush(QBrush(components::wires::WIRE_LOW_COLOR));
        }
        else if (std::static_pointer_cast<LogicButtonCell>(mLogicCell)->GetOutputState() == LogicState::HIGH)
        {
            QPen pen(components::wires::WIRE_HIGH_COLOR, components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

            pPainter->setPen(pen);
            pPainter->setBrush(QBrush(components::wires::WIRE_HIGH_COLOR));
        }

        pPainter->drawRect(mWidth * -0.25f, mHeight * -0.25f, mWidth * 0.5f, mHeight * 0.5f);
    }
}

QRectF LogicButton::boundingRect() const
{
    return QRectF(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
}
