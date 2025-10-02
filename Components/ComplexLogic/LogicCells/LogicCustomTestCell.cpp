/*!
 * \file LogicDFlipFlopCell.cpp
 * \brief Logic Cell class for the D Flip-Flop
 * \author Simon Buchholz
 * \copyright Copyright (c) 2023, Simon Buchholz
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
#include "Components/Gates/LogicCells/LogicOrGateCell.h"
#include "Components/LogicWireCell.h"
#include "HelperFunctions.h"

LogicCustomTestCell::LogicCustomTestCell():
    LogicBaseCell(3, 2)
{
    auto ha1Cell = std::make_shared<LogicHalfAdderCell>();
    auto ha2Cell = std::make_shared<LogicHalfAdderCell>();
    auto orCell = std::make_shared<LogicOrGateCell>(2);

    auto wire1Cell = std::make_shared<LogicWireCell>();
    auto wire2Cell = std::make_shared<LogicWireCell>();
    auto wire3Cell = std::make_shared<LogicWireCell>();

    mLogicCells.push_back(ha1Cell);
    mLogicCells.push_back(ha2Cell);
    mLogicCells.push_back(orCell);

    mLogicCells.push_back(wire1Cell);
    mLogicCells.push_back(wire2Cell);
    mLogicCells.push_back(wire3Cell);
}

void LogicCustomTestCell::LogicFunction()
{
    mLogicCells[0]->SetInputState(0, mInputStates[1]);
    mLogicCells[0]->SetInputState(1, mInputStates[2]);
    mLogicCells[1]->SetInputState(0, mInputStates[0]);

    for (auto& cell : mLogicCells)
    {
        cell->CalculateNextState();
    }

    for (auto& cell : mLogicCells)
    {
        cell->CommitState();
    }

    mNextOutputStates[0] = mLogicCells[1]->GetOutputState(0);
    mNextOutputStates[1] = mLogicCells[2]->GetOutputState(0);
    mStateChanged = true;
}

LogicState LogicCustomTestCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput <= 1);
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
    static_cast<LogicWireCell*>(mLogicCells[3].get())->AddInputSlot();
    static_cast<LogicWireCell*>(mLogicCells[3].get())->AppendOutput(mLogicCells[1], 1);
    mLogicCells[0]->ConnectOutput(mLogicCells[3], 0, 0);

    static_cast<LogicWireCell*>(mLogicCells[4].get())->AddInputSlot();
    static_cast<LogicWireCell*>(mLogicCells[4].get())->AppendOutput(mLogicCells[2], 1);
    mLogicCells[0]->ConnectOutput(mLogicCells[4], 0, 1);

    static_cast<LogicWireCell*>(mLogicCells[5].get())->AddInputSlot();
    static_cast<LogicWireCell*>(mLogicCells[5].get())->AppendOutput(mLogicCells[2], 0);
    mLogicCells[1]->ConnectOutput(mLogicCells[5], 0, 1);

    for (auto& cell : mLogicCells) // Wake up all inner cells
    {
        cell->OnWakeUp();
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
    for (auto& cell : mLogicCells) // Shutdown all inner cells
    {
        cell->OnShutdown();
    }

    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mCurrentOutputStates = std::vector<LogicState>{mCurrentOutputStates.size(), LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{mNextOutputStates.size(), LogicState::LOW};
    mIsActive = false;
    mStateChanged = true;
}
