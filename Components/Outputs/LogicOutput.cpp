#include "LogicOutput.h"
#include "CoreLogic.h"
#include "Configuration.h"

LogicOutput::LogicOutput(const CoreLogic* pCoreLogic):
    IBaseComponent(pCoreLogic, std::make_shared<LogicOutputCell>())
{
    setZValue(components::zvalues::OUTPUT);

    mWidth = canvas::GRID_SIZE * 0.8f;
    mHeight = canvas::GRID_SIZE * 0.8f;

    mShape.addRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);

    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 0), 0, QPointF(0, 0)));
}

LogicOutput::LogicOutput(const LogicOutput& pObj, const CoreLogic* pCoreLogic):
    LogicOutput(pCoreLogic)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
};

LogicOutput::LogicOutput(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    LogicOutput(pCoreLogic)
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));
}

IBaseComponent* LogicOutput::CloneBaseComponent(const CoreLogic* pCoreLogic) const
{
    return new LogicOutput(*this, pCoreLogic);
}

void LogicOutput::ResetZValue()
{
    setZValue(components::zvalues::OUTPUT);
}

void LogicOutput::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    if (std::static_pointer_cast<LogicOutputCell>(mLogicCell)->GetOutputState() == LogicState::LOW)
    {
        QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : components::FILL_COLOR,
                 components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::FILL_COLOR));
    }
    else if (std::static_pointer_cast<LogicOutputCell>(mLogicCell)->GetOutputState() == LogicState::HIGH)
    {
        QPen pen(components::HIGH_COLOR, components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(components::HIGH_COLOR));
    }

    pPainter->drawEllipse(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
}

QRectF LogicOutput::boundingRect() const
{
    return QRectF(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);
}

QJsonObject LogicOutput::GetJson() const
{
    QJsonObject json;

    json["type"] = file::ComponentId::OUTPUT;
    json["x"] = x();
    json["y"] = y();

    return json;
}
