/*!
 * \file LogicRsFlipFlopCell.cpp
 * \brief Logic Cell class for the RS Flip-Flop
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

#include "LogicRsFlipFlopCell.h"

LogicRsFlipFlopCell::LogicRsFlipFlopCell():
    LogicBaseCell(2, 2),
    mCurrentState(2, LogicState::LOW),
    mStateChanged(true)
{}

void LogicRsFlipFlopCell::LogicFunction()
{
    if (mInputStates[0] == LogicState::HIGH) // S high
    {
        if (mCurrentState[0] != LogicState::HIGH || mCurrentState[1] != LogicState::LOW)
        {
            mCurrentState[0] = LogicState::HIGH; // Q
            mCurrentState[1] = LogicState::LOW; // Not Q
            mStateChanged = true;
        }
    }
    if (mInputStates[1] == LogicState::HIGH) // R high
    {
        if (mCurrentState[0] != LogicState::LOW || mCurrentState[1] != LogicState::HIGH)
        {
            mCurrentState[0] = LogicState::LOW; // Q
            mCurrentState[1] = LogicState::HIGH; // Not Q
            mStateChanged = true;
        }
    }
}

LogicState LogicRsFlipFlopCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput <= 1);
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mCurrentState[pOutput]);
    }
    else
    {
        return mCurrentState[pOutput];
    }
}

void LogicRsFlipFlopCell::OnSimulationAdvance()
{
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        NotifySuccessor(0, mCurrentState[0]);
        NotifySuccessor(1, mCurrentState[1]);

        emit StateChangedSignal();
    }
}

void LogicRsFlipFlopCell::OnWakeUp()
{
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mCurrentState[0] = LogicState::LOW; // Q
    mCurrentState[1] = LogicState::HIGH; // Not Q
    mNextUpdateTime = UpdateTime::NOW;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicRsFlipFlopCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mCurrentState = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
