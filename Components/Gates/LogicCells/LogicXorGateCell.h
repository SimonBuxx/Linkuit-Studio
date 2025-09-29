#ifndef LOGICXORGATECELL_H
#define LOGICXORGATECELL_H

#include "Components/LogicBaseCell.h"

///
/// \brief Logic cell class for the XOR gate
///
class LogicXorGateCell : public LogicBaseCell
{
    Q_OBJECT
public:
    /// \brief Constructor for LogicXorGateCell
    /// \param pInputs: The number of gate inputs
    LogicXorGateCell(uint32_t pInputs);

    /// \brief The logic function that determines the output states based on the inputs
    void LogicFunction(void) override;

    /// \brief Getter for the current output state number pOutput of this cell
    /// \param pOutput: The number of the output to retreive
    /// \return The logic state of this cell's output number pOutput
    LogicState GetOutputState(uint32_t pOutput = 0) const override;

public slots:
    /// \brief Sets the in- and outputs low for edit mode and triggers a component repaint
    void OnShutdown(void) override;

    /// \brief Initializes the logic cell's states and triggers a component repaint
    void OnWakeUp(void) override;
};

#endif // LOGICXORGATECELL_H
