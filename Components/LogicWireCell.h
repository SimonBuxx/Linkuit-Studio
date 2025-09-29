#ifndef LOGICWIRECELL_H
#define LOGICWIRECELL_H

#include "LogicBaseCell.h"

class CoreLogic;

///
/// \brief Logic cell class for the logic wires and non-diode ConPoints
///
class LogicWireCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicWireCell
    LogicWireCell(void);

    /// \brief The logic function that determines the output states based on the inputs
    //void LogicFunction(void) override;

    void SetInputState(uint32_t pInput, LogicState pState) override;

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

    /// \brief Appends a logic cell as a new output to this wire cell
    /// \param pLogicCell: The logic cell to connect
    /// \param pInput: The number of the input of pCell to connect to
    void AppendOutput(const std::shared_ptr<LogicBaseCell>& pLogicCell, uint32_t pInput);

    /// \brief Adds an input slot to this wire cell where other logic cells can connect to
    void AddInputSlot(void);

    /// \brief Getter for the number of inputs to this wire cell
    /// \return The number of inputs to this wire cell
    uint32_t GetInputSize(void) const;

public slots:
    void OnCommitState(void) override;

    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;
};

#endif // LOGICWIRECELL_H
