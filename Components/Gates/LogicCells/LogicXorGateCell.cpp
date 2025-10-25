#include "LogicXorGateCell.h"
#include "HelperFunctions.h"
#include <QJsonArray>

LogicXorGateCell::LogicXorGateCell(uint32_t pInputs):
    LogicBaseCell(pInputs, 1)
{}

void LogicXorGateCell::LogicFunction()
{
    bool oneHigh = false;

    for (const auto& input : mInputStates)
    {
        if (input == LogicState::HIGH)
        {
            if (oneHigh)
            {
                if (mCurrentOutputStates[0] != LogicState::LOW)
                {
                    mNextOutputStates[0] = LogicState::LOW;
                    mStateChanged = true;
                }
                return;
            }
            oneHigh = true;
        }
    }

    if (mCurrentOutputStates[0] != (oneHigh ? LogicState::HIGH : LogicState::LOW))
    {
        mNextOutputStates[0] = oneHigh ? LogicState::HIGH : LogicState::LOW;
        mStateChanged = true;
        return;
    }
}

LogicState LogicXorGateCell::GetOutputState(uint32_t pOutput) const
{
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mCurrentOutputStates[0]);
    }
    else
    {
        return mCurrentOutputStates[0];
    }
}

QJsonObject LogicXorGateCell::ExportCell() const
{
    QJsonObject obj;

    obj["UID"] = (int32_t) mUid;
    obj["Type"] = (int32_t) file::ComponentId::XOR_GATE;

    QJsonArray ininv, outinv;

    for(const bool& inv : GetInputInversions())
    {
        ininv.append(inv);
    }

    obj["InputInversions"] = ininv;

    for(const bool& inv : GetOutputInversions())
    {
        outinv.append(inv);
    }

    obj["OutputInversions"] = outinv;

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
    obj["inputs"] = static_cast<int32_t>(GetNumInputs());

    return obj;
}

void LogicXorGateCell::OnWakeUp()
{
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mCurrentOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{1, LogicState::LOW};
    mIsActive = true;
    mStateChanged = true;
}

void LogicXorGateCell::OnShutdown()
{
    mInputStates = std::vector<LogicState>{mInputStates.size(), LogicState::LOW};
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
