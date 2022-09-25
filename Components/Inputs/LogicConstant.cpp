#include "LogicConstant.h"
#include "CoreLogic.h"
#include "Configuration.h"

LogicConstant::LogicConstant(const CoreLogic* pCoreLogic, LogicState pState):
    IBaseComponent(pCoreLogic, std::make_shared<LogicConstantCell>(pState))
{
    setZValue(components::zvalues::INPUT);

    mWidth = canvas::GRID_SIZE * 0.8f;
    mHeight = canvas::GRID_SIZE * 0.8f;

    mShape.addRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);

    mOutConnectors.push_back(LogicConnector(ConnectorType::OUT, QPointF(0, 0), 0, QPointF(0, 0))); // Place connector in the middle of the component
}

LogicConstant::LogicConstant(const LogicConstant& pObj, const CoreLogic* pCoreLogic):
    LogicConstant(pCoreLogic, pObj.GetConstantState())
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
};

LogicConstant::LogicConstant(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    LogicConstant(pCoreLogic, static_cast<LogicState>(pJson["state"].toInt()))
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));
}

IBaseComponent* LogicConstant::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new LogicConstant(*this, pCoreLogic);
}

void LogicConstant::ResetZValue()
{
    setZValue(components::zvalues::INPUT);
}

void LogicConstant::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_ASSERT(pPainter);
    Q_ASSERT(pOption);
    Q_UNUSED(pWidget);

    const double levelOfDetail = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

    if (std::static_pointer_cast<LogicConstantCell>(mLogicCell)->GetOutputState() == LogicState::LOW)
    {
        QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR,
                 components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::FILL_COLOR));
    }
    else if (std::static_pointer_cast<LogicConstantCell>(mLogicCell)->GetOutputState() == LogicState::HIGH)
    {
        QPen pen(components::HIGH_COLOR, components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::HIGH_COLOR));
    }

    pPainter->drawRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);

    // Draw state text
    if (levelOfDetail >= components::SMALL_TEXT_MIN_LOD)
    {
        auto text = (GetConstantState() == LogicState::HIGH) ? "1" : "0";

        pPainter->setPen((std::static_pointer_cast<LogicConstantCell>(mLogicCell)->GetOutputState() == LogicState::HIGH) ? components::inputs::constant::HIGH_FONT_COLOR : components::inputs::constant::LOW_FONT_COLOR);
        pPainter->setFont(components::inputs::constant::FONT);
        pPainter->drawText(boundingRect(), text, Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

QRectF LogicConstant::boundingRect() const
{
    return QRectF(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
}

QJsonObject LogicConstant::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::CONSTANT;
    json["state"] = static_cast<uint8_t>(GetConstantState());
    json["x"] = x();
    json["y"] = y();

    return json;
}

LogicState LogicConstant::GetConstantState() const
{
    return std::static_pointer_cast<LogicConstantCell>(mLogicCell)->GetConstantState();
}
