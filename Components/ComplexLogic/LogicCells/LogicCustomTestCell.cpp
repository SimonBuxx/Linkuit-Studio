/*!
 * \file LogicCustomCell.cpp
 * \brief Logic Cell class for custom logic
 * \author Simon Buchholz
 * \copyright Copyright (c) 2025, Simon Buchholz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "LogicCustomTestCell.h"
#include "Components/ComplexLogic/LogicCells/LogicHalfAdderCell.h"
#include "Components/Inputs/LogicCells/LogicInputCell.h"
#include "Components/LogicWireCell.h"
#include "Components/Outputs/LogicCells/LogicOutputCell.h"
#include "HelperFunctions.h"

LogicCustomTestCell::LogicCustomTestCell(const QString& pFileId, const CustomsLibrary& pLibrary):
    LogicBaseCell(), // in-/outputs are set later
    mFileId(pFileId),
    mLibrary(pLibrary)
{
    const auto optJson = mLibrary.GetCustomJson(pFileId);

    if (!optJson.has_value())
    {
        qDebug() << "Error: Could not configure component logic";
        return;
    }

    const auto json = optJson.value().get();
    CreateInnerCellsFromJson(*json);
    ConnectInnerCells(*json);
}

#warning split into separate functions
void LogicCustomTestCell::CreateInnerCellsFromJson(const QJsonObject& pConfig)
{
    if (!pConfig.contains("Cells") || !pConfig["Cells"].isArray())
    {
        qWarning("JSON object does not contain a 'Cells' array.");
        return;
    }

    // Create all inner logic cells before connecting them

    QJsonArray cellsArray = pConfig["Cells"].toArray();

    for (const QJsonValue& cellValue : cellsArray)
    {
        QJsonObject cellObj = cellValue.toObject();

        switch ((file::ComponentId) cellObj["Type"].toInt())
        {
        case file::ComponentId::WIRE:
        {
            qDebug() << "Creating wire cell with UID" << cellObj["UID"].toInt();
            auto cell = std::make_shared<LogicWireCell>();

            cell->SetInnerCell();
            cell->SetUid(cellObj["UID"].toInt());
            mInnerCells.insert(std::pair{cell->GetUid(), cell});
            break;
        }
        case file::ComponentId::HALF_ADDER:
        {
            qDebug() << "Creating half adder cell with UID" << cellObj["UID"].toInt();
            auto cell = std::make_shared<LogicHalfAdderCell>();
            cell->SetInnerCell();

            // Set input inversions
            {
                std::vector<bool> inversions{};
#warning TODO: guard all JSON calls with contains() and cancel init if info is missing
                for (const QJsonValue& val : cellObj["InputInversions"].toArray())
                {
                    inversions.push_back(val.toBool());
                }
                cell->SetInputInversions(inversions);
            }

            // Set output inversions
            {
                std::vector<bool> inversions{};
                for (const QJsonValue& val : cellObj["OutputInversions"].toArray())
                {
                    inversions.push_back(val.toBool());
                }
                cell->SetOutputInversions(inversions);
            }

            cell->SetUid(cellObj["UID"].toInt());
            mInnerCells.insert(std::pair{cell->GetUid(), cell});
            break;
        }
        case file::ComponentId::INPUT:
        {
            qDebug() << "Creating input cell with UID" << cellObj["UID"].toInt();
            auto cell = std::make_shared<LogicInputCell>();
            cell->SetInnerCell();

            mInputStates.push_back(LogicState::LOW);
            mInputConnected.push_back(false);
            mInputInverted.push_back(false);

            cell->SetUid(cellObj["UID"].toInt());
            mInnerCells.insert(std::pair{cell->GetUid(), cell});

            mInputCellUids.insert(std::pair{cell->GetUid(), mInputStates.size() - 1});

            break;
        }
        case file::ComponentId::OUTPUT:
        {
            qDebug() << "Creating output cell with UID" << cellObj["UID"].toInt();
            auto cell = std::make_shared<LogicOutputCell>();
            cell->SetInnerCell();

            mCurrentOutputStates.push_back(LogicState::LOW);
            mNextOutputStates.push_back(LogicState::LOW);
            mOutputInverted.push_back(false);
            mOutputCells.push_back(std::make_pair(nullptr, 0));

            cell->SetUid(cellObj["UID"].toInt());
            mInnerCells.insert(std::pair{cell->GetUid(), cell});

            mOutputCellUids.insert(std::pair{cell->GetUid(), mCurrentOutputStates.size() - 1});

            break;
        }
        case file::ComponentId::CUSTOM_LOGIC:
        {
            qDebug() << "Creating custom cell with UID" << cellObj["UID"].toInt() << "and file name" << cellObj["FileName"].toString();

            auto cell = std::make_shared<LogicCustomTestCell>(cellObj["FileName"].toString(), mLibrary);
            cell->SetInnerCell();

            // Set input inversions
            {
                std::vector<bool> inversions{};
                for (const QJsonValue& val : cellObj["InputInversions"].toArray())
                {
                    inversions.push_back(val.toBool());
                }
                cell->SetInputInversions(inversions);
            }

            // Set output inversions
            {
                std::vector<bool> inversions{};
                for (const QJsonValue& val : cellObj["OutputInversions"].toArray())
                {
                    inversions.push_back(val.toBool());
                }
                cell->SetOutputInversions(inversions);
            }

            cell->SetUid(cellObj["UID"].toInt());
            mInnerCells.insert(std::pair{cell->GetUid(), cell});

            break;
        }
        default:
        {
            break;
        }
        }
    }
}

void LogicCustomTestCell::ConnectInnerCells(const QJsonObject& pConfig)
{
    QJsonArray cellsArray = pConfig["Cells"].toArray();

    for (const QJsonValue& cellValue : cellsArray)
    {
        QJsonObject cellObj = cellValue.toObject();

        // Loop through all cells and connect their outputs to other cell's inputs

        for (const QJsonValue& val : cellObj["OutputCells"].toArray())
        {
            QJsonArray triple = val.toArray();

            Q_ASSERT(triple.size() == 3);
            Q_ASSERT(mInnerCells.count(triple[0].toInt()) > 0); // Check connected cell
            Q_ASSERT(mInnerCells.count(cellObj["UID"].toInt()) > 0); // Check current cell

            auto&& connectedCell = mInnerCells.at(triple[0].toInt());
            auto&& connectedInput = triple[1].toInt();
            auto&& output = triple[2].toInt();

            if (cellObj["Type"].toInt() == file::ComponentId::WIRE)
            {
                qDebug() << "Adding an output for wire cell with UID" << cellObj["UID"].toInt() << "to connected cell with UID" << triple[0].toInt() << ", input #" << connectedInput;
                std::static_pointer_cast<LogicWireCell>(mInnerCells.at(cellObj["UID"].toInt()))->AppendOutput(connectedCell, connectedInput);
            }
            else
            {
                qDebug() << "Connecting output" << output << "of logic cell with UID" << cellObj["UID"].toInt() << "to connected cell with UID" << triple[0].toInt() << ", input #" << connectedInput;

                if (dynamic_cast<LogicWireCell*>(connectedCell.get()) != nullptr) // If it's a wire cell
                {
                    qDebug() << "Adding input slot first";
                    // Create input slot first
                    std::static_pointer_cast<LogicWireCell>(connectedCell)->AddInputSlot();
                }
                mInnerCells.at(cellObj["UID"].toInt())->ConnectOutput(connectedCell, connectedInput, output);
            }
        }
    }
}

void LogicCustomTestCell::LogicFunction()
{
    // Forward input states
    for (auto& input : mInputCellUids)
    {
        std::static_pointer_cast<LogicInputCell>(mInnerCells[input.first])->SetState(mInputStates[input.second]);

        // Advance input state so that inputs do not introduce delay
        mInnerCells[input.first]->CalculateNextState();
        mInnerCells[input.first]->CommitState();
    }

    // Update inner cells

    for (auto& cell : mInnerCells)
    {
        cell.second->CalculateNextState();
    }

    for (auto& cell : mInnerCells)
    {
        cell.second->CommitState();
    }

    // Extract output states
    for (auto& output : mOutputCellUids)
    {
        // Advance output state so that outputs do not introduce delay
        mInnerCells[output.first]->CalculateNextState();
        mInnerCells[output.first]->CommitState();
        mNextOutputStates[output.second] = std::static_pointer_cast<LogicOutputCell>(mInnerCells[output.first])->GetOutputState();
    }

    mStateChanged = true;
}

LogicState LogicCustomTestCell::GetOutputState(uint32_t pOutput) const
{
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mCurrentOutputStates[pOutput]);
    }
    else
    {
        return mCurrentOutputStates[pOutput];
    }
}

void LogicCustomTestCell::OnWakeUp()
{
    for (auto& cell : mInnerCells) // Wake up all inner cells
    {
        cell.second->OnWakeUp();
    }

    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mCurrentOutputStates = std::vector<LogicState>{mCurrentOutputStates.size(), LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{mNextOutputStates.size(), LogicState::LOW};
    mIsActive = true;
    mStateChanged = true;
}

void LogicCustomTestCell::OnShutdown()
{
    for (auto& cell : mInnerCells) // Shutdown all inner cells
    {
        cell.second->OnShutdown();
    }

    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mCurrentOutputStates = std::vector<LogicState>{mCurrentOutputStates.size(), LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{mNextOutputStates.size(), LogicState::LOW};

    if (!mIsInnerCell)
    {
        mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
        mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    }

    mIsActive = false;
    mStateChanged = true;
}
