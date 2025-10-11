/*!
 * \file LogicCustomCell.h
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
#ifndef LOGICCUSTOMCELL_H
#define LOGICCUSTOMCELL_H

#include "Components/LogicBaseCell.h"
#include "CustomsLibrary.h"
#include <QJsonObject>
#include <QJsonArray>

///
/// \brief Logic Cell class for the custom logic cell
///
class LogicCustomCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for the custom logic cell
    LogicCustomCell(const CircuitId& pCircuitId, const CustomsLibrary& pLibrary);

    /// \brief The logic function that determines the output states based on the inputs
    void LogicFunction(void) override;

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retrieve
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

    QJsonObject ExportCell(void) const override;

protected:
    /// \brief This function creates all inner logic cell from the JSON provided
    /// \param pConfig: The JSON object to create the logic cells from
    void CreateInnerCellsFromJson(const QJsonObject& pConfig);

    /// \brief This function connects the existing inner logic cells according to the JSON provided
    /// \param pConfig: The JSON object according to which to connect the logic cells
    void ConnectInnerCells(const QJsonObject& pConfig);

    /// \brief Creates and insert a logic cell for a logic wire
    /// \param pConfig: The configuration for the logic cell
    void CreateWireCell(const QJsonObject& pConfig);

    /// \brief Creates and insert a logic cell for an AND gate
    /// \param pConfig: The configuration for the logic cell
    void CreateAndGateCell(const QJsonObject& pConfig);

    /// \brief Creates and insert a logic cell for a logic input
    /// \param pConfig: The configuration for the logic cell
    void CreateInputCell(const QJsonObject& pConfig);

    /// \brief Creates and insert a logic cell for a logic button
    /// \param pConfig: The configuration for the logic cell
    void CreateButtonCell(const QJsonObject& pConfig);

    /// \brief Creates and insert a logic cell for a logic clock
    /// \param pConfig: The configuration for the logic cell
    void CreateClockCell(const QJsonObject& pConfig);

    /// \brief Creates and insert a logic cell for a logic output
    /// \param pConfig: The configuration for the logic cell
    void CreateOutputCell(const QJsonObject& pConfig);

    /// \brief Creates and insert a logic cell for a half adder
    /// \param pConfig: The configuration for the logic cell
    void CreateHalfAdderCell(const QJsonObject& pConfig);

    /// \brief Creates and insert a logic cell for custom logic
    /// \param pConfig: The configuration for the logic cell
    void CreateCustomLogicCell(const QJsonObject& pConfig);

public slots:
    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;

protected:
    std::map<uint32_t, std::shared_ptr<LogicBaseCell>> mInnerCells; // {UID, Cell}

    std::map<uint32_t, uint32_t> mInputCellUids; // {UID, Input#}
    std::map<uint32_t, uint32_t> mOutputCellUids; // {UID, Output#}

    CircuitId mCircuitId;
    const CustomsLibrary& mLibrary;
};

#endif // LOGICCUSTOMCELL_H
