#ifndef LOGICBASECELL_H
#define LOGICBASECELL_H

#include "HelperFunctions.h"

#include <QObject>

/// \brief UpdateTime contains values to indicate when the logic cell should update its state
enum class UpdateTime
{
    NOW = 0,
    NEXT_TICK,
    INF
};

///
/// \brief The LogicBaseCell class is the abstract base for all logic cells
///
class LogicBaseCell: public QObject
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicBaseCell
    /// \param pInputs: The amount of inputs to this cell
    /// \param pOutputs: The amount of outputs from this cell
    LogicBaseCell(uint32_t pInputs, uint32_t pOutputs);

    /// \brief Default destructor for LogicBaseCell
    virtual ~LogicBaseCell() = default;

    /// \brief The logic function that determines the output states based on the inputs
    virtual void LogicFunction(void) {};

    /// \brief Notifies the logic cell connected to output pOutput to update its input state to pState
    /// \param pOutput: The number of the output that has changed to pState
    /// \param pState: The new state of the output
    void NotifySuccessor(uint32_t pOutput, LogicState pState) const;

    /// \brief Sets input number pInput to the new state pState
    /// \param pInput: The number of the changed input
    /// \param pState: The new state of the input
    virtual void InputReady(uint32_t pInput, LogicState pState);

    /// \brief Sets logic cell pLogicCell's input number pInput to this cell's output number pOutput
    /// \param pLogicCell: The logic cell to connect to this cell's output
    /// \param pInput: The number of the input of the connected cell
    /// \param pOutput: The output number of this cell to connect
    void ConnectOutput(const std::shared_ptr<LogicBaseCell>& pLogicCell, uint32_t pInput, uint32_t pOutput);

    /// \brief Getter for the current input state number pInput of this cell
    /// \param pInput: The number of the input to retreive
    /// \return The logic state of this cell's input number pInput
    LogicState GetInputState(uint32_t pInput) const;

    /// \brief Checks if the given cell input is inverted
    /// \param pInput: A number of a cell input
    /// \return True, if the input is inverted
    bool IsInputInverted(uint32_t pInput) const;

    void InvertInput(uint32_t pInput);

    /// \brief Getter for the current input inversion vector
    /// \return The current input inversion vector
    std::vector<bool> GetInputInversions(void) const;

    /// \brief Sets the input inversion vector of this cell
    /// \param pInputInversions: The new input inversion vector
    void SetInputInversions(const std::vector<bool>& pInputInversions);

    /// \brief Checks if the given cell output is inverted
    /// \param pOutput: A number of a cell output
    /// \return True, if the output is inverted
    bool IsOutputInverted(uint32_t pOutput) const;

    void InvertOutput(uint32_t pOutput);

    /// \brief Getter for the current output inversion vector
    /// \return The current output inversion vector
    std::vector<bool> GetOutputInversions(void) const;

    /// \brief Sets the output inversion vector of this cell
    /// \param pOutputInversions: The new output inversion vector
    void SetOutputInversions(const std::vector<bool>& pOutputInversions);

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    virtual LogicState GetOutputState(uint32_t pOutput = 0) const = 0;

    /// \brief Returns true, if this logic cell is not shut down
    /// \return True, if logic cell active
    bool IsActive(void) const;

protected:
    /// \brief If the mNextUpdateTime value is NOW, AdvanceUpdateTime calls LogicFunction()
    /// If it's NEXT_TICK, the update time is advanced to NOW
    void AdvanceUpdateTime(void);

    /// \brief Inverts the given state if pOutput is an inverted output
    /// \param pState: A logic state
    /// \param pOutput: A number of a cell output
    /// \return The output logic state
    LogicState ApplyInversion(LogicState pState, uint32_t pOutput) const;

public slots:
    /// \brief Advances the simulation of this cell by one logic tick
    virtual void OnSimulationAdvance(void) {};

    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    virtual void OnShutdown(void) {};

    /// \brief Initializes the logic cell's states and triggers a component repaint
    virtual void OnWakeUp(void) {};

signals:
    /// \brief Emitted when the output state of this cell changed
    void StateChangedSignal(void);

protected:
    std::vector<LogicState> mInputStates;
    std::vector<bool> mInputInverted;
    std::vector<bool> mOutputInverted;

    // Pairs of connected LogicCell and input number of that cell
    std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>> mOutputCells;

    UpdateTime mNextUpdateTime;

    bool mIsActive;
};

#endif // LOGICBASECELL_H
