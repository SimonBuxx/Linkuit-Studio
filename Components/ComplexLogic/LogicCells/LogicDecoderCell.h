#ifndef LOGICDECODERCELL_H
#define LOGICDECODERCELL_H

#include "Components/LogicBaseCell.h"

///
/// \brief Logic Cell class for the decoder
///
class LogicDecoderCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for the decoder cell
    /// \param pInputCount: The amount of inputs for this decoder
    LogicDecoderCell(uint8_t pInputCount);

    /// \brief The logic function that determines the output states based on the inputs
    void LogicFunction(void) override;

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    /// \brief Advances the simulation of this cell by one logic tick
    void OnSimulationAdvance(void) override;

    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;

protected:
    std::vector<LogicState> mOutputStates;

    bool mStateChanged;

    uint8_t mInputCount;
    uint8_t mPreviousValue;
};

#endif // LOGICDECODERCELL_H
