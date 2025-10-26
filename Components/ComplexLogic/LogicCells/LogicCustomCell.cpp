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

#include "LogicCustomCell.h"
#include "Components/ComplexLogic/LogicCells/LogicCounterCell.h"
#include "Components/ComplexLogic/LogicCells/LogicDFlipFlopCell.h"
#include "Components/ComplexLogic/LogicCells/LogicDMsFlipFlopCell.h"
#include "Components/ComplexLogic/LogicCells/LogicDecoderCell.h"
#include "Components/ComplexLogic/LogicCells/LogicDemultiplexerCell.h"
#include "Components/ComplexLogic/LogicCells/LogicEncoderCell.h"
#include "Components/ComplexLogic/LogicCells/LogicFullAdderCell.h"
#include "Components/ComplexLogic/LogicCells/LogicHalfAdderCell.h"
#include "Components/ComplexLogic/LogicCells/LogicJKFlipFlopCell.h"
#include "Components/ComplexLogic/LogicCells/LogicJkMsFlipFlopCell.h"
#include "Components/ComplexLogic/LogicCells/LogicMultiplexerCell.h"
#include "Components/ComplexLogic/LogicCells/LogicRsClockedFlipFlopCell.h"
#include "Components/ComplexLogic/LogicCells/LogicRsFlipFlopCell.h"
#include "Components/ComplexLogic/LogicCells/LogicRsMsFlipFlopCell.h"
#include "Components/ComplexLogic/LogicCells/LogicShiftRegisterCell.h"
#include "Components/ComplexLogic/LogicCells/LogicTFlipFlopCell.h"
#include "Components/Gates/LogicCells/LogicAndGateCell.h"
#include "Components/Gates/LogicCells/LogicBufferGateCell.h"
#include "Components/Gates/LogicCells/LogicNotGateCell.h"
#include "Components/Gates/LogicCells/LogicOrGateCell.h"
#include "Components/Gates/LogicCells/LogicXorGateCell.h"
#include "Components/Inputs/LogicCells/LogicClockCell.h"
#include "Components/Inputs/LogicCells/LogicConstantCell.h"
#include "Components/Inputs/LogicCells/LogicInputCell.h"
#include "Components/LogicDiodeCell.h"
#include "Components/LogicWireCell.h"
#include "Components/Outputs/LogicCells/LogicOutputCell.h"
#include "HelperFunctions.h"

LogicCustomCell::LogicCustomCell(const CircuitId& pCircuitId, const CustomsLibrary& pLibrary):
    LogicBaseCell(), // in-/outputs are set later
    mCircuitId(pCircuitId),
    mLibrary(pLibrary)
{
    const auto optJson = mLibrary.GetCustomJson(pCircuitId);

    if (!optJson.has_value())
    {
        qDebug() << "Error: Could not configure component logic";
        return;
    }

    const auto json = optJson.value().get();
    CreateInnerCellsFromJson(*json);
    ConnectInnerCells(*json);
}

void LogicCustomCell::CreateInnerCellsFromJson(const QJsonObject& pConfig)
{
    if (!pConfig.contains("Cells") || !pConfig["Cells"].isArray())
    {
        qWarning("JSON object does not contain a 'Cells' array.");
        return;
    }

    QJsonArray cellsArray = pConfig["Cells"].toArray();

    for (const QJsonValue& cellValue : std::as_const(cellsArray))
    {
        const QJsonObject cellObj = cellValue.toObject();

        if (!cellObj.contains("UID") || !cellObj["UID"].isDouble())
        {
            qDebug() << "Error: Logic cell to load has no UID";
            continue;
        }

        if (!cellObj.contains("Type") || !cellObj["Type"].isDouble())
        {
            qDebug() << "Error: Logic cell to load has no type";
            continue;
        }

        switch ((file::ComponentId) cellObj["Type"].toInt())
        {
        case file::ComponentId::WIRE:
        {
            CreateWireCell(cellObj);
            break;
        }
        case file::ComponentId::CONPOINT:
        {
            CreateConPointCell(cellObj);
            break;
        }
        case file::ComponentId::AND_GATE:
        {
            CreateLogicCell_VariableInputCount<LogicAndGateCell>(cellObj, components::gates::DEFAULT_INPUT_COUNT);
            break;
        }
        case file::ComponentId::OR_GATE:
        {
            CreateLogicCell_VariableInputCount<LogicOrGateCell>(cellObj, components::gates::DEFAULT_INPUT_COUNT);
            break;
        }
        case file::ComponentId::XOR_GATE:
        {
            CreateLogicCell_VariableInputCount<LogicXorGateCell>(cellObj, components::gates::DEFAULT_INPUT_COUNT);
            break;
        }
        case file::ComponentId::NOT_GATE:
        {
            CreateLogicCell<LogicNotGateCell>(cellObj);
            break;
        }
        case file::ComponentId::BUFFER_GATE:
        {
            CreateLogicCell<LogicBufferGateCell>(cellObj);
            break;
        }
        case file::ComponentId::INPUT:
        {
            CreateInputCell(cellObj);
            break;
        }
        case file::ComponentId::BUTTON:
        {
            CreateInputCell(cellObj);
            break;
        }
        case file::ComponentId::CLOCK:
        {
            CreateClockCell(cellObj);
            break;
        }
        case file::ComponentId::OUTPUT:
        {
            CreateOutputCell(cellObj);
            break;
        }
        case file::ComponentId::TEXT_LABEL:
        {
            break; // Ignore inner text labels
        }
        case file::ComponentId::HALF_ADDER:
        {
            CreateLogicCell<LogicHalfAdderCell>(cellObj);
            break;
        }
        case file::ComponentId::FULL_ADDER:
        {
            CreateLogicCell<LogicFullAdderCell>(cellObj);
            break;
        }
        case file::ComponentId::RS_FLIPFLOP:
        {
            CreateLogicCell<LogicRsFlipFlopCell>(cellObj);
            break;
        }
        case file::ComponentId::D_FLIPFLOP:
        {
            CreateLogicCell<LogicDFlipFlopCell>(cellObj);
            break;
        }
        case file::ComponentId::MULTIPLEXER:
        {
            CreateLogicCell_VariableBitWidth<LogicMultiplexerCell>(cellObj, components::multiplexer::DEFAULT_BIT_WIDTH);
            break;
        }
        case file::ComponentId::DEMULTIPLEXER:
        {
            CreateLogicCell_VariableBitWidth<LogicDemultiplexerCell>(cellObj, components::multiplexer::DEFAULT_BIT_WIDTH);
            break;
        }
        case file::ComponentId::T_FLIPFLOP:
        {
            CreateLogicCell<LogicTFlipFlopCell>(cellObj);
            break;
        }
        case file::ComponentId::JK_FLIPFLOP:
        {
            CreateLogicCell<LogicJKFlipFlopCell>(cellObj);
            break;
        }
        case file::ComponentId::DECODER:
        {
            CreateLogicCell_VariableInputCount<LogicDecoderCell>(cellObj, components::encoder_decoder::DEFAULT_INPUT_COUNT);
            break;
        }
        case file::ComponentId::ENCODER:
        {
            CreateLogicCell_VariableOutputCount<LogicEncoderCell>(cellObj, components::encoder_decoder::DEFAULT_INPUT_COUNT);
            break;
        }
        case file::ComponentId::SHIFTREGISTER:
        {
            CreateLogicCell_VariableBitWidth<LogicShiftRegisterCell>(cellObj, components::shift_register::DEFAULT_BIT_WIDTH);
            break;
        }
        case file::ComponentId::CONSTANT:
        {
            CreateConstantLogicCell(cellObj);
            break;
        }
        case file::ComponentId::COUNTER:
        {
            CreateLogicCell_VariableBitWidth<LogicCounterCell>(cellObj, components::counter::DEFAULT_BIT_WIDTH);
            break;
        }
        case file::ComponentId::RS_MS_FLIPFLOP:
        {
            CreateLogicCell<LogicRsMsFlipFlopCell>(cellObj);
            break;
        }
        case file::ComponentId::RS_CLOCKED_FLIPFLOP:
        {
            CreateLogicCell<LogicRsClockedFlipFlopCell>(cellObj);
            break;
        }
        case file::ComponentId::D_MS_FLIPFLOP:
        {
            CreateLogicCell<LogicDMsFlipFlopCell>(cellObj);
            break;
        }
        case file::ComponentId::JK_MS_FLIPFLOP:
        {
            CreateLogicCell<LogicJkMsFlipFlopCell>(cellObj);
            break;
        }
        case file::ComponentId::CUSTOM_LOGIC:
        {
            CreateCustomLogicCell(cellObj);
            break;
        }
        default:
        {
            qDebug() << "Error: Component inside a custom logic component has unknown type";
            continue;
        }
        }
    }
}

void LogicCustomCell::CreateWireCell(const QJsonObject& pConfig)
{
    auto cell = std::make_shared<LogicWireCell>();
    cell->SetInnerCell();

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});
}

void LogicCustomCell::CreateConPointCell(const QJsonObject& pConfig)
{
    auto cell = std::make_shared<LogicDiodeCell>();
    cell->SetInnerCell();

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});
}

template <typename T>
void LogicCustomCell::CreateLogicCell(const QJsonObject& pConfig)
{
    std::shared_ptr<LogicBaseCell> cell = std::make_shared<T>();
    cell->SetInnerCell();

    SetCellInversions(pConfig, cell);

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});
}

template <typename T>
void LogicCustomCell::CreateLogicCell_VariableInputCount(const QJsonObject& pConfig, uint8_t pDefaultInputCount)
{
    // Fetch input count, if available
    uint8_t inputs = pDefaultInputCount;
    if (pConfig.contains("inputs") && pConfig["inputs"].isDouble())
    {
        inputs = pConfig["inputs"].toInt();
    }

    std::shared_ptr<LogicBaseCell> cell = std::make_shared<T>(inputs);
    cell->SetInnerCell();

    SetCellInversions(pConfig, cell);

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});
}

template <typename T>
void LogicCustomCell::CreateLogicCell_VariableOutputCount(const QJsonObject& pConfig, uint8_t pDefaultOutputCount)
{
    // Fetch output count, if available
    uint8_t outputs = pDefaultOutputCount;
    if (pConfig.contains("outputs") && pConfig["outputs"].isDouble())
    {
        outputs = pConfig["outputs"].toInt();
    }

    std::shared_ptr<LogicBaseCell> cell = std::make_shared<T>(outputs);
    cell->SetInnerCell();

    SetCellInversions(pConfig, cell);

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});
}

template <typename T>
void LogicCustomCell::CreateLogicCell_VariableBitWidth(const QJsonObject& pConfig, uint8_t pDefaultBitWidth)
{
    // Fetch bit width, if available
    uint8_t bits = pDefaultBitWidth;
    if (pConfig.contains("bits") && pConfig["bits"].isDouble())
    {
        bits = pConfig["bits"].toInt();
    }

    std::shared_ptr<LogicBaseCell> cell = std::make_shared<T>(bits);
    cell->SetInnerCell();

    SetCellInversions(pConfig, cell);

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});
}

void LogicCustomCell::CreateInputCell(const QJsonObject& pConfig)
{
    std::shared_ptr<LogicBaseCell> cell = std::make_shared<LogicInputCell>();
    cell->SetInnerCell();

    // This cell is an input to the custom logic, append it as such
    mInputStates.push_back(LogicState::LOW);
    mInputConnected.push_back(false);
    mInputInverted.push_back(false);

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});

    // Add cell to the input cells
    mInputCellUids.insert(std::pair{cell->GetUid(), mInputStates.size() - 1});
}

void LogicCustomCell::CreateClockCell(const QJsonObject& pConfig)
{
    auto cell = std::make_shared<LogicClockCell>();
    cell->SetInnerCell();

    cell->SetUid(pConfig["UID"].toInt());

    // Set clock-specific parameters
    if (pConfig.contains("mode") && pConfig["mode"].isDouble())
    {
        cell->SetClockMode((ClockMode) pConfig["mode"].toInt());
    }

    if (pConfig.contains("toggle") && pConfig["toggle"].isDouble())
    {
        cell->SetToggleTicks(pConfig["toggle"].toInt());
    }

    if (pConfig.contains("pulse") && pConfig["pulse"].isDouble())
    {
        cell->SetPulseTicks(pConfig["pulse"].toInt());
    }

    mInnerCells.insert(std::pair{cell->GetUid(), cell});
}

void LogicCustomCell::CreateOutputCell(const QJsonObject& pConfig)
{
    auto cell = std::make_shared<LogicOutputCell>();
    cell->SetInnerCell();

    mCurrentOutputStates.push_back(LogicState::LOW);
    mNextOutputStates.push_back(LogicState::LOW);
    mOutputInverted.push_back(false);
    mOutputCells.push_back(std::make_pair(nullptr, 0));

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});

    mOutputCellUids.insert(std::pair{cell->GetUid(), mCurrentOutputStates.size() - 1});
}

void LogicCustomCell::CreateConstantLogicCell(const QJsonObject& pConfig)
{

    LogicState state = LogicState::HIGH;
    if (pConfig.contains("state") && pConfig["state"].isDouble())
    {
        state = static_cast<LogicState>(pConfig["state"].toInt());
    }

    auto cell = std::make_shared<LogicConstantCell>(state);
    cell->SetInnerCell();

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});

    // Note: Constants are not brought out of the custom logic as inputs
}

void LogicCustomCell::CreateCustomLogicCell(const QJsonObject& pConfig)
{
    if (!pConfig.contains("uuid") || !pConfig["uuid"].isString())
    {
        qDebug() << "Error: Nested custom logic does not contain a UUID";
        return;
    }

    if (!pConfig.contains("timestamp") || !pConfig["timestamp"].isString())
    {
        qDebug() << "Error: Nested custom logic does not contain a timestamp";
        return;
    }

    std::shared_ptr<LogicBaseCell> cell = std::make_shared<LogicCustomCell>(CircuitId(pConfig["uuid"].toString(),
                                                                pConfig["timestamp"].toInt()), mLibrary);
    cell->SetInnerCell();

    SetCellInversions(pConfig, cell);

    cell->SetUid(pConfig["UID"].toInt());
    mInnerCells.insert(std::pair{cell->GetUid(), cell});

}

void LogicCustomCell::SetCellInversions(const QJsonObject& pConfig, std::shared_ptr<LogicBaseCell>& pCell)
{
    // Set input inversions
    if (pConfig.contains("InputInversions") && pConfig["InputInversions"].isArray())
    {
        std::vector<bool> inversions{};
        const QJsonArray ininv = pConfig["InputInversions"].toArray();
        for (const QJsonValue& val : ininv)
        {
            inversions.push_back(val.toBool());
        }
        pCell->SetInputInversions(inversions);
    }

    // Set output inversions
    if (pConfig.contains("OutputInversions") && pConfig["OutputInversions"].isArray())
    {
        std::vector<bool> inversions{};
        const QJsonArray outinv = pConfig["OutputInversions"].toArray();
        for (const QJsonValue& val : outinv)
        {
            inversions.push_back(val.toBool());
        }
        pCell->SetOutputInversions(inversions);
    }
}

void LogicCustomCell::ConnectInnerCells(const QJsonObject& pConfig)
{
    if (!pConfig.contains("Cells") || !pConfig["Cells"].isArray())
    {
        qDebug() << "Error: Cancelling inner logic cell connection because Cells field is missing";
        return;
    }

    QJsonArray cellsArray = pConfig["Cells"].toArray();
    for (const QJsonValue& cellValue : std::as_const(cellsArray))
    {
        QJsonObject cellObj = cellValue.toObject();

        if (!cellObj.contains("UID") || !cellObj["UID"].isDouble())
        {
            qDebug() << "Error: Could not connect inner logic cells because a cell does not contain a UID";
            continue;
        }

        if (!cellObj.contains("Type") || !cellObj["Type"].isDouble())
        {
            qDebug() << "Error: Could not connect inner logic cells because a cell does not contain a type";
            continue;
        }

        // Loop through all cells and connect their outputs to other cell's inputs
        if (cellObj.contains("OutputCells") && cellObj["OutputCells"].isArray())
        {
            const QJsonArray outputCells = cellObj["OutputCells"].toArray();
            for (const QJsonValue& val : outputCells)
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
                    std::static_pointer_cast<LogicWireCell>(mInnerCells.at(cellObj["UID"].toInt()))->AppendOutput(connectedCell, connectedInput);
                }
                else
                {
                    if (dynamic_cast<LogicWireCell*>(connectedCell.get()) != nullptr) // If it's a wire cell
                    {
                        std::static_pointer_cast<LogicWireCell>(connectedCell)->AddInputSlot(); // Create input slot first
                    }
                    mInnerCells.at(cellObj["UID"].toInt())->ConnectOutput(connectedCell, connectedInput, output);
                }
            }
        }
    }
}

void LogicCustomCell::LogicFunction()
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

QJsonObject LogicCustomCell::ExportCell() const
{
    QJsonObject obj;

    obj["UID"] = (int32_t) mUid;
    obj["Type"] = (int32_t) file::ComponentId::CUSTOM_LOGIC;

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
    obj["uuid"] = std::get<0>(mCircuitId);
    obj["timestamp"] = std::get<1>(mCircuitId);

    return obj;
}

LogicState LogicCustomCell::GetOutputState(uint32_t pOutput) const
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

void LogicCustomCell::OnWakeUp()
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

void LogicCustomCell::OnShutdown()
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
