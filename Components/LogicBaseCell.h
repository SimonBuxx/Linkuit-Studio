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
    ~LogicBaseCell() override = default;

    /// \brief The logic function that determines the output states based on the inputs
    virtual void LogicFunction(void) {};

    /// \brief Sets logic cell pLogicCell's input number pInput to this cell's output number pOutput
    /// \param pLogicCell: The logic cell to connect to this cell's output
    /// \param pInput: The number of the input of the connected cell
    /// \param pOutput: The output number of this cell to connect
    void ConnectOutput(const std::shared_ptr<LogicBaseCell>& pLogicCell, uint32_t pInput, uint32_t pOutput);

    /// \brief Marks the input with the given index as connected
    /// \param pInput: The index of the input
    void SetConnected(uint32_t pInput);

    /// \brief Returns true, if the given input is connected to another logic cell
    /// \param pInput: The index of the input
    /// \return True, if the given input is connected to another logic cell
    bool IsInputConnected(uint32_t pInput);

    /// \brief Returns true, if the given output is connected to another logic cell
    /// \param pOutput: The index of the output
    /// \return True, if the given output is connected to another logic cell
    bool IsOutputConnected(uint32_t pOutput);

    /// \brief Getter for the current input state number pInput of this cell
    /// \param pInput: The number of the input to retreive
    /// \return The logic state of this cell's input number pInput
    LogicState GetInputState(uint32_t pInput) const;

    /// \brief Checks if the given cell input is inverted
    /// \param pInput: A number of a cell input
    /// \return True, if the input is inverted
    bool IsInputInverted(uint32_t pInput) const;

    /// \brief Inverts the input with index pInput
    /// \param pInput: The input index to invert
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

    /// \brief Inverts the output with index pOutput
    /// \param pOutput: The output index to invert
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

    virtual void SetInputState(uint32_t pInput, LogicState pState);

protected:
    /// \brief Sets the state pSubject to the state pTargetState if it isn't in that state already
    /// Returns true if pSubject wasn't already in pTargetState
    /// \param pSubject: Reference to the state to set
    /// \param pTargetState: The target state
    /// \return True, if the state has been changed
    bool AssureState(LogicState &pSubject, const LogicState &pTargetState);

    /// \brief Sets the state pSubject to the state pTargetState if it isn't in that state already
    /// and if the given condition is true
    /// Returns true if pSubject wasn't already in pTargetState
    /// \param pCondition: The condition to be true for the state to be assured
    /// \param pSubject: Reference to the state to set
    /// \param pTargetState: The target state
    /// \return True, if the state has been changed
    bool AssureStateIf(bool pCondition, LogicState &pSubject, const LogicState &pTargetState);

public slots:
    virtual void OnCalculateNextState(void);

    virtual void OnCommitState(void);

    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    virtual void OnShutdown(void) {};

    /// \brief Initializes the logic cell's states and triggers a component repaint
    virtual void OnWakeUp(void) {};

signals:
    /// \brief Emitted when the output state of this cell changed
    void StateChangedSignal(void);

protected:
    std::vector<LogicState> mInputStates;
    std::vector<bool> mInputConnected;
    std::vector<bool> mInputInverted;
    std::vector<bool> mOutputInverted;

    // Pairs of connected LogicCell and input number of that cell
    std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>> mOutputCells;

    std::vector<LogicState> mCurrentOutputStates;
    std::vector<LogicState> mNextOutputStates;

#warning remove
    UpdateTime mNextUpdateTime;

    bool mIsActive;

    bool mStateChanged;
};

#endif // LOGICBASECELL_H
