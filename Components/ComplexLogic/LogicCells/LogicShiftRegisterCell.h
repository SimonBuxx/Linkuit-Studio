#ifndef LOGICSHIFTREGISTERCELL_H
#define LOGICSHIFTREGISTERCELL_H

#include "Components/LogicBaseCell.h"

///
/// \brief Logic Cell class for the shift register
///
class LogicShiftRegisterCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for the shift register cell
    /// \param pBitWidth: The size of the shift register in bits
    LogicShiftRegisterCell(uint8_t pBitWidth);

    /// \brief The logic function that determines the output states based on the inputs
    void LogicFunction(void) override;

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

    /// \brief Getter for the current output state number pOutput of this cell (ignoring inversion)
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput (ignoring inversion)
    LogicState GetOutputStateUninverted(uint32_t pOutput = 0) const;

    /// \brief Sets input number pInput to the new state pState
    /// \param pInput: The number of the changed input
    /// \param pState: The new state of the input
    virtual void InputReady(uint32_t pInput, LogicState pState) override;

public slots:
    /// \brief Advances the simulation of this cell by one logic tick
    void OnSimulationAdvance(void) override;

    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;

protected:
    std::vector<LogicState> mOutputStates;
    std::vector<LogicState> mPrevInputStates;

    bool mStateChanged;

    uint8_t mBitWidth;
};

#endif // LOGICSHIFTREGISTERCELL_H
