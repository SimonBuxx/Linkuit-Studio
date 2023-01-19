/*!
 * \file LogicDemultiplexerCell.cpp
 * \brief Logic Cell class for the demultiplexer
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

#include "LogicDemultiplexerCell.h"

LogicDemultiplexerCell::LogicDemultiplexerCell(uint8_t pDigitCount):
    LogicBaseCell(pDigitCount + 1, std::pow(2, pDigitCount)),
    mOutputStates(std::pow(2, pDigitCount), LogicState::LOW),
    mStateChanged(true),
    mDigitCount(pDigitCount),
    mPreviousOutput(0)
{}

void LogicDemultiplexerCell::LogicFunction()
{    
    uint32_t output = 0;

    for (size_t i = 0; i < mDigitCount; i++)
    {
        output += (mInputStates[i] == LogicState::HIGH) ? std::pow(2, i) : 0;
    }

    mStateChanged |= AssureStateIf(output != mPreviousOutput, mOutputStates[mPreviousOutput], LogicState::LOW);
    mStateChanged |= AssureState(mOutputStates[output], mInputStates[mDigitCount]);
    mPreviousOutput = output;
}

LogicState LogicDemultiplexerCell::GetOutputState(uint32_t pOutput) const
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

void LogicDemultiplexerCell::OnSimulationAdvance()
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

void LogicDemultiplexerCell::OnWakeUp()
{   
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mNextUpdateTime = UpdateTime::NOW;

    mPreviousOutput = 0;

    mStateChanged = true; // Successors should be notified about wake up
    mIsActive = true;
    emit StateChangedSignal();
}

void LogicDemultiplexerCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mOutputStates = std::vector<LogicState>(mOutputStates.size(), LogicState::LOW);
    mIsActive = false;
    emit StateChangedSignal();
}
