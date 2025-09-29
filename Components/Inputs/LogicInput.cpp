#include "LogicInput.h"
#include "CoreLogic.h"
#include "Configuration.h"
#include "HelperFunctions.h"

LogicInput::LogicInput(const CoreLogic* pCoreLogic):
    IBaseComponent(pCoreLogic, std::make_shared<LogicInputCell>())
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

LogicInput::LogicInput(const LogicInput& pObj, const CoreLogic* pCoreLogic):
    LogicInput(pCoreLogic)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
};

LogicInput::LogicInput(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    LogicInput(pCoreLogic)
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));
}

IBaseComponent* LogicInput::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new LogicInput(*this, pCoreLogic);
}

void LogicInput::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    Q_ASSERT(pEvent);

    if (mSimulationRunning)
    {
        std::static_pointer_cast<LogicInputCell>(mLogicCell)->ToggleState();
    }
    IBaseComponent::mousePressEvent(pEvent);
}

void LogicInput::ResetZValue()
{
    setZValue(components::zvalues::INPUT);
}

void LogicInput::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_ASSERT(pPainter);
    Q_ASSERT(pOption);
    Q_UNUSED(pWidget);

    if (std::static_pointer_cast<LogicInputCell>(mLogicCell)->GetOutputState() == LogicState::LOW)
    {
        QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR,
                 components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::FILL_COLOR));
    }
    else if (std::static_pointer_cast<LogicInputCell>(mLogicCell)->GetOutputState() == LogicState::HIGH)
    {
        QPen pen(components::HIGH_COLOR, components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::HIGH_COLOR));
    }

    pPainter->drawRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
}

QRectF LogicInput::boundingRect() const
{
    return QRectF(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
}

QJsonObject LogicInput::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::INPUT;
    json["x"] = x();
    json["y"] = y();

    return json;
}

SwVersion LogicInput::GetMinVersion(void) const
{
    return SwVersion(0, 0, 0);
}
