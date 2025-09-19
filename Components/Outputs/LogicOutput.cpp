#include "LogicOutput.h"
#include "CoreLogic.h"
#include "Configuration.h"
#include "LogicCells/LogicOutputCell.h"

LogicOutput::LogicOutput(const CoreLogic* pCoreLogic, OutputColor pColor):
    IBaseComponent(pCoreLogic, std::make_shared<LogicOutputCell>()),
    mColor(pColor)
{
    setZValue(components::zvalues::OUTPUT);

    mWidth = canvas::GRID_SIZE * 0.8f;
    mHeight = canvas::GRID_SIZE * 0.8f;

    mShape.addRect(mWidth * -0.5f, mHeight * -0.5f, mWidth, mHeight);

    mInConnectors.push_back(LogicConnector(ConnectorType::IN, QPointF(0, 0), 0, QPointF(0, 0)));
}

LogicOutput::LogicOutput(const LogicOutput& pObj, const CoreLogic* pCoreLogic):
    LogicOutput(pCoreLogic, pObj.mColor)
{
    mWidth = pObj.mWidth;
    mHeight = pObj.mHeight;
};

LogicOutput::LogicOutput(const CoreLogic* pCoreLogic, const QJsonObject& pJson):
    LogicOutput(pCoreLogic)
{
    setPos(SnapToGrid(QPointF(pJson["x"].toInt(), pJson["y"].toInt())));
    if (pJson.contains("color"))
    {
        if (pJson["color"] == static_cast<int32_t>(OutputColor::DEFAULT))
        {
            mColor = OutputColor::DEFAULT;
        }
        else if (pJson["color"] == static_cast<int32_t>(OutputColor::GREEN))
        {
            mColor = OutputColor::GREEN;
        }
        else if (pJson["color"] == static_cast<int32_t>(OutputColor::RED))
        {
            mColor = OutputColor::RED;
        }
        else if (pJson["color"] == static_cast<int32_t>(OutputColor::BLUE))
        {
            mColor = OutputColor::BLUE;
        }
        else if (pJson["color"] == static_cast<int32_t>(OutputColor::YELLOW))
        {
            mColor = OutputColor::YELLOW;
        }
        else
        {
            throw std::logic_error("Unknown output color");
        }
    }
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

    QColor color;

    if (std::static_pointer_cast<LogicOutputCell>(mLogicCell)->GetOutputState() == LogicState::LOW)
    {
        switch (mColor)
        {
        case OutputColor::DEFAULT:
            color = components::outputs::DEFAULT_COLOR_LOW;
            break;
        case OutputColor::GREEN:
            color = components::outputs::GREEN_COLOR_LOW;
            break;
        case OutputColor::RED:
            color = components::outputs::RED_COLOR_LOW;
            break;
        case OutputColor::BLUE:
            color = components::outputs::BLUE_COLOR_LOW;
            break;
        case OutputColor::YELLOW:
            color = components::outputs::YELLOW_COLOR_LOW;
            break;
        default:
            throw std::logic_error("Output color invalid");
            break;
        }

        QPen pen(pOption->state & QStyle::State_Selected ? components::SELECTED_BORDER_COLOR : color,
                 components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(color));

    }
    else if (std::static_pointer_cast<LogicOutputCell>(mLogicCell)->GetOutputState() == LogicState::HIGH)
    {
        switch (mColor)
        {
        case OutputColor::DEFAULT:
            color = components::outputs::DEFAULT_COLOR_HIGH;
            break;
        case OutputColor::GREEN:
            color = components::outputs::GREEN_COLOR_HIGH;
            break;
        case OutputColor::RED:
            color = components::outputs::RED_COLOR_HIGH;
            break;
        case OutputColor::BLUE:
            color = components::outputs::BLUE_COLOR_HIGH;
            break;
        case OutputColor::YELLOW:
            color = components::outputs::YELLOW_COLOR_HIGH;
            break;
        default:
            throw std::logic_error("Output color invalid");
            break;
        }
        QPen pen(color, components::BORDER_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        pPainter->setPen(pen);
        pPainter->setBrush(QBrush(color));
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
    json["color"] = static_cast<int32_t>(mColor);

    return json;
}

SwVersion LogicOutput::GetMinVersion(void) const
{
    return SwVersion(0, 0, 0);
}
