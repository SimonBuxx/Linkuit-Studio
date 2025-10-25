#include "LogicDiodeCell.h"
#include <QJsonArray>

LogicDiodeCell::LogicDiodeCell():
    LogicBaseCell(1, 1) // Diodes always have exactly one input wire and one output wire
{
}

void LogicDiodeCell::SetInputState(uint32_t pInput, LogicState pState)
{
    LogicBaseCell::SetInputState(pInput, pState);

    LogicState newOutputState = LogicState::LOW;

    if (mCurrentOutputStates[0] != mInputStates[0])
    {
        newOutputState = mInputStates[0];

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

LogicState LogicDiodeCell::GetOutputState(uint32_t pOutput) const
{
    Q_UNUSED(pOutput);
    return mCurrentOutputStates[0];
}

QJsonObject LogicDiodeCell::ExportCell() const
{
    QJsonObject obj;

    obj["UID"] = (int32_t) mUid;
    obj["Type"] = (int32_t) file::ComponentId::CONPOINT;

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

void LogicDiodeCell::OnWakeUp()
{
    mInputStates = std::vector<LogicState>{LogicState::LOW};
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = true;
    mStateChanged = true;
}

void LogicDiodeCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{LogicState::LOW};
    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};

    if (!mIsInnerCell)
    {
        mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
        mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    }

    mIsActive = false;
    mStateChanged = true;
}
