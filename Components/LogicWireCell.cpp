#include "LogicWireCell.h"
#include <QJsonArray>

LogicWireCell::LogicWireCell():
    LogicBaseCell(0, 1)
{}

void LogicWireCell::SetInputState(uint32_t pInput, LogicState pState)
{
    LogicBaseCell::SetInputState(pInput, pState);

    LogicState newOutputState = LogicState::LOW;

    for (const auto& inputState : mInputStates)
    {
        if (inputState == LogicState::HIGH)
        {
            newOutputState = LogicState::HIGH;
            break;
        }
    }

    if (mCurrentOutputStates[0] != newOutputState)
    {
        emit StateChangedSignal();

        mCurrentOutputStates[0] = newOutputState;
        mNextOutputStates[0] = newOutputState;

        for (const auto& outputConnection : mOutputCells)
        {
            if (outputConnection.first != nullptr)
            {
                outputConnection.first->SetInputState(outputConnection.second, newOutputState);
            }
        }
    }
}

void LogicWireCell::AppendOutput(const std::shared_ptr<LogicBaseCell>& pLogicCell, uint32_t pInput)
{
    mOutputCells.push_back(std::make_pair(pLogicCell, pInput));
    mOutputInverted.push_back(false);
    pLogicCell->SetConnected(pInput);
}

void LogicWireCell::AddInputSlot()
{
    mInputStates.push_back(LogicState::LOW);
    mInputInverted.push_back(false);
}

uint32_t LogicWireCell::GetInputSize() const
{
    return mInputStates.size();
}

LogicState LogicWireCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentOutputStates[0];
}

QJsonObject LogicWireCell::ExportCell() const
{
    QJsonObject obj;

    obj["UID"] = (int32_t) mUid;
    obj["Type"] = (int32_t) file::ComponentId::WIRE;

    // Store connections
    QJsonArray outputCells;

    for (size_t output = 0; output < mOutputCells.size(); output++)
    {
        if (mOutputCells[output].first != nullptr) // Output connected
        {
            QJsonArray connection;

            connection.append((int32_t) mOutputCells[output].first->GetUid()); // UID
            connection.append((int32_t) mOutputCells[output].second); // Remote input
            connection.append((int32_t) output); // Local output

            outputCells.append(connection);
        }
    }

    obj["OutputCells"] = outputCells;

    return obj;
}

void LogicWireCell::OnWakeUp()
{
    if (!mIsInnerCell)
    {
        mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
        mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    }
    else
    {
        mCurrentOutputStates = std::vector<LogicState>{mCurrentOutputStates.size(), LogicState::LOW};
        mNextOutputStates = std::vector<LogicState>{mNextOutputStates.size(), LogicState::LOW};
    }
    mIsActive = true;
    mStateChanged = true;
}

void LogicWireCell::OnShutdown()
{
    if (!mIsInnerCell)
    {
        mOutputCells.clear();
        mInputStates.clear();
        mInputConnected.clear();
        mOutputInverted.clear();
        mInputInverted.clear();
        mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
        mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    }
    else
    {
        mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
        /*mInputConnected = std::vector<bool>(mInputConnected.size(), false);
        mOutputInverted = std::vector<bool>(mOutputInverted.size(), false);
        mInputInverted = std::vector<bool>(mInputInverted.size(), false);*/
        mCurrentOutputStates = std::vector<LogicState>{mCurrentOutputStates.size(), LogicState::LOW};
        mNextOutputStates = std::vector<LogicState>{mNextOutputStates.size(), LogicState::LOW};
    }
    mIsActive = false;
    mStateChanged = true;
}
