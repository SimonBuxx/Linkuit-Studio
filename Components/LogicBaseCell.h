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

    /// \brief Sets logic cell pCell's input number pInput to this cell's output number pOutput
    /// \param pCell: The logic cell to connect to this cell's output
    /// \param pInput: The number of the input of the connected cell
    /// \param pOutput: The output number of this cell to connect
    void ConnectOutput(std::shared_ptr<LogicBaseCell> pCell, uint32_t pInput, uint32_t pOutput);

    /// \brief Getter for the curent input state number pInput of this cell
    /// \param pInput: The number of the input to retreive
    /// \return The logic state of this cell's input number pInput
    LogicState GetInputState(uint32_t pInput) const;

    bool IsInputInverted(uint32_t pInput) const;

    std::vector<bool> GetInputInversions(void) const;

    void SetInputInversions(std::vector<bool> pInputInversions);

    /// \brief Getter for the curent output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    virtual LogicState GetOutputState(uint32_t pOutput = 0) const = 0;

protected:
    /// \brief If the mNextUpdateTime value is NOW, AdvanceUpdateTime calls LogicFunction()
    /// If it's NEXT_TICK, the update time is advanced to NOW
    void AdvanceUpdateTime(void);

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

    // Pairs of connected LogicCell and input number of that cell
    std::vector<std::pair<std::shared_ptr<LogicBaseCell>, uint32_t>> mOutputCells;

    UpdateTime mNextUpdateTime;
};

#endif // LOGICBASECELL_H
