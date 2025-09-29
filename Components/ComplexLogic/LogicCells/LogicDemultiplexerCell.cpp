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
#include "HelperFunctions.h"

LogicDemultiplexerCell::LogicDemultiplexerCell(uint8_t pDigitCount):
    LogicBaseCell(pDigitCount + 1, std::pow(2, pDigitCount)),
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

    if (output != mPreviousOutput && mCurrentOutputStates[mPreviousOutput] == LogicState::LOW)
    {
        mNextOutputStates[mPreviousOutput] = LogicState::LOW;
        mStateChanged = true;
    }

    if (mCurrentOutputStates[output] != mInputStates[mDigitCount])
    {
        mNextOutputStates[output] = mInputStates[mDigitCount];
        mStateChanged = true;
    }

    mPreviousOutput = output;
}

LogicState LogicDemultiplexerCell::GetOutputState(uint32_t pOutput) const
{
    Q_ASSERT(pOutput < mCurrentOutputStates.size());
    if (mOutputInverted[pOutput] && mIsActive)
    {
        return InvertState(mCurrentOutputStates[pOutput]);
    }
    else
    {
        return mCurrentOutputStates[pOutput];
    }
}

void LogicDemultiplexerCell::OnWakeUp()
{   
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);

    for (size_t i = 0; i < mInputStates.size(); i++)
    {
        mInputStates[i] = mInputInverted[i] ? LogicState::HIGH : LogicState::LOW;
    }

    mCurrentOutputStates = std::vector<LogicState>{mCurrentOutputStates.size(), LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{mNextOutputStates.size(), LogicState::LOW};

    mPreviousOutput = 0;
    mIsActive = true;
    mStateChanged = true;
}

void LogicDemultiplexerCell::OnShutdown()
{
    mOutputCells = std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>>(mOutputCells.size(), std::make_pair(nullptr, 0));
    mInputStates = std::vector<LogicState>(mInputStates.size(), LogicState::LOW);
    mInputConnected = std::vector<bool>(mInputConnected.size(), false);
    mCurrentOutputStates = std::vector<LogicState>{mCurrentOutputStates.size(), LogicState::LOW};
    mNextOutputStates = std::vector<LogicState>{mNextOutputStates.size(), LogicState::LOW};
    mIsActive = false;
    mStateChanged = true;
}
