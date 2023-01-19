/*!
 * \file LogicCounterCell.cpp
 * \brief Logic Cell class for the synchronous counter
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

#include "LogicCounterCell.h"

LogicCounterCell::LogicCounterCell(uint8_t pBitWidth):
    LogicBaseCell(3, pBitWidth),
    mOutputStates(pBitWidth, LogicState::LOW),
    mPrevInputStates(pBitWidth, LogicState::LOW),
    mStateChanged(true),
    mBitWidth(pBitWidth),
    mMaxValue(std::pow(2, pBitWidth) - 1)
{}

void LogicCounterCell::LogicFunction()
{
    if ((mPrevInputStates[2] == LogicState::LOW && mInputStates[2] == LogicState::HIGH))
    {
        if (mInputStates[0] == LogicState::HIGH)
        {
            mValue = 0;
        }
        else if (mInputStates[1] == LogicState::LOW)
        {
            mValue = (mValue < mMaxValue) ? mValue + 1: 0;
        }

        uint32_t remainder = 0;
        uint32_t val = mValue;
        for (uint8_t i = 0; i < mBitWidth; i++)
        {
            remainder = val % 2;
            val /= 2;
            auto newState = (remainder == 1) ? LogicState::HIGH : LogicState::LOW;
            if (mOutputStates[i] != newState)
            {
                mOutputStates[i] = newState;
                mStateChanged = true;
            }
        }
    }

    if (mPrevInputStates[2] != mInputStates[2]) // Trigger repaint on every clock change
    {
        mStateChanged = true;
    }

    mPrevInputStates = mInputStates;
}

LogicState LogicCounterCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput < mOutputStates.size());
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mOutputStates[pOutput]);
    }
    else
    {
        return mOutputStates[pOutput];
    }
}

void LogicCounterCell::OnSimulationAdvance()
{
    AdvanceUpdateTime();

    if (mStateChanged)
    {
        mStateChanged = false;
        for (size_t i = 0; i < mOutputStates.size(); i++)
        {
            NotifySuccessor(i, mOutputStates[i]);
        }

        emit StateChangedSignal();
    }
}

void LogicCounterCell::InputReady(uint32_t pInput, LogicState pState)
{
    if (mInputStates[pInput] != pState)
    {
        emit StateChangedSignal(); // to trigger immediate update of the component
    }
    LogicBaseCell::InputReady(pInput, pState);
}

void LogicCounterCell::OnWakeUp()
{
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mPrevInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mNextUpdateTime = UpdateTime::NOW;
    mValue = 0;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicCounterCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
